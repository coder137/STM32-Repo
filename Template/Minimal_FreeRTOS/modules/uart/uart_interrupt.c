#include "uart_interrupt.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

// TODO, Make this into an enum
// static const uint8_t CR1_IDLEIE = 4;
static const uint8_t CR1_RXINE = 5;
const uint8_t CR1_TCIE = 6;
const uint8_t CR1_TXEIE = 7;
// static const uint8_t CR1_PEIE = 8;
// static const uint8_t CR1_CMIE = 14;
// static const uint8_t CR1_RTOIE = 26;
// static const uint8_t CR1_EOBIE = 27;

static const uint8_t ISR_RXNE = 5;
static const uint8_t ISR_TC = 6;
static const uint8_t ISR_TXE = 7;

// STATIC FUNCTION DEFINITION
static void
uart_interrupt_process_helper(const UART_interrupt_s *interrupt_config,
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

// FUNCTION
void uart_interrupt__init(UART_interrupt_s *interrupt_config) {

  interrupt_config->_internal.rx_queue =
      xQueueCreate(interrupt_config->rx_queue_length, sizeof(uint8_t));
  interrupt_config->_internal.tx_queue =
      xQueueCreate(interrupt_config->tx_queue_length, sizeof(uint8_t));

  // RX Interrupt
  interrupt_config->usart->CR1 |= ((1 << CR1_RXINE));

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

uint8_t uart_interrupt__read(const UART_interrupt_s *interrupt_config) {
  uint8_t data = 0;
  xQueueReceive(interrupt_config->_internal.rx_queue, &data, portMAX_DELAY);
  return data;
}

// TODO, Return Success or Error code
void uart_interrupt__process(const UART_interrupt_s *interrupt_config) {

  // TODO, Process Errors here

  // ELSE do the rest here
  // RXNE
  uart_interrupt_process_helper(interrupt_config, ISR_RXNE,
                                uart_interrupt__process_rxne);

  // TXE
  uart_interrupt_process_helper(interrupt_config, ISR_TXE,
                                uart_interrupt_process_txe);

  // TC
  uart_interrupt_process_helper(interrupt_config, ISR_TC,
                                uart_interrupt__process_tc);

  // TODO, Return some value here
}

static void uart_interrupt__process_rxne(UART_interrupt_s *interrupt_config) {
  BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
  uint8_t data = (uint8_t)interrupt_config->usart->RDR;
  xQueueSendFromISR(interrupt_config->_internal.rx_queue, &data,
                    &pxHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}

static void uart_interrupt_process_txe(UART_interrupt_s *interrupt_config) {
  uint8_t data;
  BaseType_t is_recv =
      xQueueReceiveFromISR(interrupt_config->_internal.tx_queue, &data, NULL);
  if (is_recv) {
    interrupt_config->usart->TDR = data;
  } else {
    interrupt_config->usart->CR1 &= ~(1 << CR1_TXEIE);
  }
}

static void uart_interrupt__process_tc(UART_interrupt_s *interrupt_config) {
  // Disable this interrupt
  interrupt_config->usart->CR1 &= ~(1 << CR1_TCIE);

  // TODO, Run a function here
}

static void
uart_interrupt_process_helper(const UART_interrupt_s *interrupt_config,
                              uint8_t pin,
                              void (*interrupt_func)(UART_interrupt_s *)) {
  USART_TypeDef *usart = interrupt_config->usart;
  bool is_interrupted = (usart->ISR >> pin) & 0x01;
  bool interrupt_enabled = (usart->CR1 >> pin) & 0x01;

  if (is_interrupted && interrupt_enabled) {
    // Run function pointer
    if (interrupt_func != NULL) {
      interrupt_func((UART_interrupt_s *)interrupt_config);
    }
  }
}

static void uart_interrupt__write_activate(UART_interrupt_s *interrupt_config) {
  USART_TypeDef *usart = interrupt_config->usart;
  uint32_t cr1_data = usart->CR1;
  uint8_t txe_enabled = (cr1_data >> CR1_TXEIE) & 0x01;
  uint8_t tc_enabled = (cr1_data >> CR1_TCIE) & 0x01;
  uint8_t combined = (txe_enabled << 1) | tc_enabled;

  switch (combined) {
  case 0:
    // TXE and TC are off
    cr1_data |= ((1 << CR1_TXEIE) | (1 << CR1_TCIE));
    usart->CR1 = cr1_data;
    break;
  case 1:
    // TXE just turned off, TC is still ON
    cr1_data |= ((1 << CR1_TXEIE));
    usart->CR1 = cr1_data;
    break;
  case 3:
    // Both are active
    break;
  case 2:
  // NOTE, Should not happen since we will never deactivate tc before txe
  default:
    break;
  }
}

static void uart_interrupt__internal_write(UART_interrupt_s *interrupt_config,
                                           uint8_t data) {
  xQueueSend(interrupt_config->_internal.tx_queue, &data, portMAX_DELAY);
}

static void
uart_interrupt__internal_write_string(UART_interrupt_s *interrupt_config,
                                      char *data) {
  while (*data != 0) {
    xQueueSend(interrupt_config->_internal.tx_queue, data, portMAX_DELAY);
    data++;
  }
}
