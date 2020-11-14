/**
 * Transmission Explanation
 * TXE is raised when data CAN be loaded again
 * TC is raised when data is successfully transmitted (per frame)
 *
 */
#include "uart_interrupt.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

// TODO, Make this into an enum
// static const uint8_t CR1_IDLEIE = 4;
static const uint8_t CR1_RXINE = 5;
static const uint8_t CR1_TCIE = 6;
static const uint8_t CR1_TXEIE = 7;
// static const uint8_t CR1_PEIE = 8;
// static const uint8_t CR1_CMIE = 14;
// static const uint8_t CR1_RTOIE = 26;
// static const uint8_t CR1_EOBIE = 27;

static const uint8_t ISR_RXNE = 5;
static const uint8_t ISR_TC = 6;
static const uint8_t ISR_TXE = 7;

// STATIC FUNCTION DEFINITION
// Init functionality
static void uart_interrupt__receive_enable(UART_interrupt_s *interrupt_config);
static void uart_interrupt__transmit_enable(UART_interrupt_s *interrupt_config);

// Interrupt functionality
static void
uart_interrupt__process_helper(const UART_interrupt_s *interrupt_config,
                               uint8_t pin,
                               void (*interrupt_func)(UART_interrupt_s *));
static void uart_interrupt__process_rxne(UART_interrupt_s *interrupt_config);
static void uart_interrupt_process_txe(UART_interrupt_s *interrupt_config);
static void uart_interrupt__process_tc(UART_interrupt_s *interrupt_config);

// Write functionality
static void uart_interrupt__write_activate(UART_interrupt_s *interrupt_config);
static void uart_interrupt__internal_write(UART_interrupt_s *interrupt_config,
                                           uint8_t data);
static void
uart_interrupt__internal_write_string(UART_interrupt_s *interrupt_config,
                                      char *data);

// Event functionality
static void uart_interrupt__callback_notify(UART_interrupt_s *interrupt_config,
                                            UART_interrupt_event_e event);

// FUNCTION
void uart_interrupt__init(UART_interrupt_s *interrupt_config) {

  interrupt_config->rx_queue =
      xQueueCreate(interrupt_config->rx_queue_length, sizeof(uint8_t));
  interrupt_config->tx_queue =
      xQueueCreate(interrupt_config->tx_queue_length, sizeof(uint8_t));

  // RX Interrupt
  uart_interrupt__receive_enable(interrupt_config);

  // This is for FreeRTOS kernel APIs to be used within ISR.
  // ISR priority is set to be lower that the FreeRTOS kernel (higher number) so
  // that we can call the FreeRTOS ISR functions
  NVIC_SetPriority(USART1_IRQn,
                   configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
  NVIC_EnableIRQ(USART1_IRQn);
}

void uart_interrupt__write(UART_interrupt_s *interrupt_config, uint8_t data) {
  // First write the data
  uart_interrupt__internal_write(interrupt_config, data);

  // Then activate the interrupts
  uart_interrupt__write_activate(interrupt_config);
}

void uart_interrupt__write_string(UART_interrupt_s *interrupt_config,
                                  char *data) {
  // First write the data
  uart_interrupt__internal_write_string(interrupt_config, data);

  // Then activate the interrupts
  uart_interrupt__write_activate(interrupt_config);
}

void uart_interrupt__write_string_n(UART_interrupt_s *interrupt_config,
                                    char *data, size_t n) {
  // First write the data
  for (size_t i = 0; i < n; i++) {
    uart_interrupt__internal_write(interrupt_config, data[i]);
  }

  // Then activate the interrupts
  uart_interrupt__write_activate(interrupt_config);
}

uint8_t uart_interrupt__read(const UART_interrupt_s *interrupt_config,
                             uint32_t wait_for) {
  uint8_t data = 0;
  xQueueReceive(interrupt_config->rx_queue, &data, wait_for);
  return data;
}

// NOTE, We need to handle each individual bit in an if statement since multiple
// bits can be set in the ISR register
// Cannot make an FSM here
void uart_interrupt__process(const UART_interrupt_s *interrupt_config) {

  // TODO, Process Errors here
  // TODO, Send events here

  // ELSE do the rest here
  // RXNE
  uart_interrupt__process_helper(interrupt_config, ISR_RXNE,
                                 uart_interrupt__process_rxne);

  // TXE
  uart_interrupt__process_helper(interrupt_config, ISR_TXE,
                                 uart_interrupt_process_txe);

  // TC (Transmission Complete)
  uart_interrupt__process_helper(interrupt_config, ISR_TC,
                                 uart_interrupt__process_tc);
}

// Init Functionality
static void uart_interrupt__receive_enable(UART_interrupt_s *interrupt_config) {
  UART_s *uart_config = interrupt_config->uart_config;
  switch (uart_config->communication_mode) {
  case UART_communication_mode_RX_ONLY:
  case UART_communication_mode_RX_TX:
    uart_config->usart->CR1 |= (1 << CR1_RXINE);
    break;
  case UART_communication_mode_INACTIVE:
  case UART_communication_mode_TX_ONLY:
  default:
    // do nothing
    break;
  }
}

static void
uart_interrupt__transmit_enable(UART_interrupt_s *interrupt_config) {
  UART_s *uart_config = interrupt_config->uart_config;
  switch (uart_config->communication_mode) {
  case UART_communication_mode_TX_ONLY:
  case UART_communication_mode_RX_TX:
    uart_config->usart->CR1 |= ((1 << CR1_TXEIE) | (1 << CR1_TCIE));
    break;
  case UART_communication_mode_INACTIVE:
  case UART_communication_mode_RX_ONLY:
  default:
    // do nothing
    break;
  }
}

// Interrupt Functionality
static void uart_interrupt__process_rxne(UART_interrupt_s *interrupt_config) {
  BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
  uint8_t data = (uint8_t)interrupt_config->uart_config->usart->RDR;
  xQueueSendFromISR(interrupt_config->rx_queue, &data,
                    &pxHigherPriorityTaskWoken);
  uart_interrupt__callback_notify(interrupt_config,
                                  UART_interrupt_event_RX_READY);
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}

static void uart_interrupt_process_txe(UART_interrupt_s *interrupt_config) {
  uint8_t data;
  BaseType_t is_recv =
      xQueueReceiveFromISR(interrupt_config->tx_queue, &data, NULL);
  if (is_recv) {
    interrupt_config->uart_config->usart->TDR = data;
    uart_interrupt__callback_notify(interrupt_config,
                                    UART_interrupt_event_TX_COMPLETE);
  } else {
    // If there is no more data, disable the "Transmit data register empty"
    // interrupt and "Transmission Complete"
    interrupt_config->uart_config->usart->CR1 &=
        ~((1 << CR1_TXEIE) | (1 << CR1_TCIE));

    // Message transmission complete
    uart_interrupt__callback_notify(interrupt_config,
                                    UART_interrupt_event_TRANSFER_COMPLETE);
  }
}

static void uart_interrupt__process_tc(UART_interrupt_s *interrupt_config) {
  uart_interrupt__callback_notify(interrupt_config,
                                  UART_interrupt_event_SEND_FRAME_COMPLETE);
}

// `interrupt_func` should never be NULL
// Since this is for internal use, NULL guard has not been placed
static void
uart_interrupt__process_helper(const UART_interrupt_s *interrupt_config,
                               uint8_t pin,
                               void (*interrupt_func)(UART_interrupt_s *)) {
  USART_TypeDef *usart = interrupt_config->uart_config->usart;
  const bool is_interrupted = (usart->ISR >> pin) & 0x01;
  const bool interrupt_enabled = (usart->CR1 >> pin) & 0x01;

  if (is_interrupted && interrupt_enabled) {
    interrupt_func((UART_interrupt_s *)interrupt_config);
  }
}

static void uart_interrupt__write_activate(UART_interrupt_s *interrupt_config) {
  const USART_TypeDef *usart = interrupt_config->uart_config->usart;
  const uint32_t cr1_data = usart->CR1;
  const uint8_t txe_enabled = (cr1_data >> CR1_TXEIE) & 0x01;
  const uint8_t tc_enabled = (cr1_data >> CR1_TCIE) & 0x01;
  const uint8_t combined = (txe_enabled << 1) | tc_enabled;

  // FSM only created for verbosity!
  switch (combined) {
  case 0:
    // TXE and TC are off
    uart_interrupt__transmit_enable(interrupt_config);
    break;
  case 1:
  case 2:
    // Both interrupts are enabled and disabled at the same time, This case
    // should never be hit
    break;
  case 3:
    // Both are active
    break;
  default:
    break;
  }
}

static void uart_interrupt__internal_write(UART_interrupt_s *interrupt_config,
                                           uint8_t data) {
  xQueueSend(interrupt_config->tx_queue, &data, portMAX_DELAY);
}

static void
uart_interrupt__internal_write_string(UART_interrupt_s *interrupt_config,
                                      char *data) {
  while (*data != 0) {
    uart_interrupt__internal_write(interrupt_config, *data);
    data++;
  }
}

static void uart_interrupt__callback_notify(UART_interrupt_s *interrupt_config,
                                            UART_interrupt_event_e event) {
  if (interrupt_config->UART_event_cb != NULL) {
    interrupt_config->UART_event_cb(event);
  }
}
