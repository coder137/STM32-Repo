#include "uart_interrupt.h"

#include "gpio.h"
#include "uart.h"

#include <stdio.h>

// Constants
static const uint8_t LCR_DLAB = 7;

static const uint8_t IER_RBRIE = 0;
static const uint8_t IER_THREIE = 1;
static const uint8_t IER_RXIE = 2;

static const uint8_t LSR_THRE = 5;

static const uint8_t IIR_INTSTATUS = 0;
static const uint8_t IIR_INTID = 1;

// Static functions

// Write
static void uart_interrupt__internal_write(UART_interrupt_s *interrupt_config,
                                           uint8_t data);
static void uart_interrupt__activate_write(UART_interrupt_s *interrupt_config);

// Interrupts
static void uart_interrupt__handle_thre(UART_interrupt_s *interrupt_config);
static void uart_interrupt__handle_rda(UART_interrupt_s *interrupt_config);
static void uart_interrupt__handle_errors(UART_interrupt_s *interrupt_config);
static void uart_interrupt__callback_notify(UART_interrupt_s *interrupt_config,
                                            UART_interrupt_event_e event);

void uart_interrupt__init(UART_interrupt_s *interrupt_config) {
  interrupt_config->rx_queue =
      xQueueCreate(interrupt_config->rx_queue_length, sizeof(uint8_t));
  interrupt_config->tx_queue =
      xQueueCreate(interrupt_config->tx_queue_length, sizeof(uint8_t));

  USART_TypeDef *usart = interrupt_config->uart_config->usart;

  // Make sure DLAB is set to 0
  usart->LCR &= ~(1 << LCR_DLAB);

  // Start your interrupts here
  const uint32_t set = ((1 << IER_THREIE) | (1 << IER_RBRIE) | (1 << IER_RXIE));
  uint32_t ier_data = usart->IER;
  ier_data |= set;
  usart->IER = ier_data;

  // Start transmission with a empty bit
  usart->THR = 0;
}

void uart_interrupt__write(UART_interrupt_s *interrupt_config, uint8_t data) {
  uart_interrupt__internal_write(interrupt_config, data);
  uart_interrupt__activate_write(interrupt_config);
}

void uart_interrupt__write_string(UART_interrupt_s *interrupt_config,
                                  char *data) {
  while (*data != 0) {
    uart_interrupt__internal_write(interrupt_config, *data);
    data++;
  }
  uart_interrupt__activate_write(interrupt_config);
}

void uart_interrupt__write_string_n(UART_interrupt_s *interrupt_config,
                                    char *data, size_t n) {
  for (int i = 0; i < n; i++) {
    uart_interrupt__internal_write(interrupt_config, data[i]);
  }
  uart_interrupt__activate_write(interrupt_config);
}

uint8_t uart_interrupt__read(const UART_interrupt_s *interrupt_config,
                             uint32_t wait_for) {
  uint8_t data = 0;
  xQueueReceive(interrupt_config->rx_queue, (void *)&data, wait_for);
  return data;
}

void uart_interrupt__process(UART_interrupt_s *interrupt_config) {
  USART_TypeDef *usart = interrupt_config->uart_config->usart;

  // No interrupt is pending
  uint32_t iir_data = usart->IIR;
  if (((iir_data >> IIR_INTSTATUS) & 0x01) == 1) {
    return;
  }

  uint8_t status = (iir_data >> IIR_INTID) & 0x7;
  switch (status) {
  case 0x1:
    // THRE
    uart_interrupt__handle_thre(interrupt_config);
    break;
  case 0x2:
    // RDA (Receive data available)
    uart_interrupt__handle_rda(interrupt_config);
    break;
  case 0x3:
    // RLS (Receive Line Status)
    uart_interrupt__handle_errors(interrupt_config);
    while (1) {
    }
    break;
  case 0x6:
    // CTI(Character Timeout Indicator)
  default: {
    uint32_t lsr_data = usart->LSR;
    (void)lsr_data;
    break;
  }
  }
}

// Static functions

// Write
static void uart_interrupt__internal_write(UART_interrupt_s *interrupt_config,
                                           uint8_t data) {
  xQueueSend(interrupt_config->tx_queue, &data, portMAX_DELAY);
}

static void uart_interrupt__activate_write(UART_interrupt_s *interrupt_config) {
  interrupt_config->uart_config->usart->IER |= (1 << IER_THREIE);
}

// Interrupt
static void uart_interrupt__handle_thre(UART_interrupt_s *interrupt_config) {
  BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
  USART_TypeDef *usart = interrupt_config->uart_config->usart;
  uint32_t lsr_data = usart->LSR;
  bool is_empty = (lsr_data >> LSR_THRE) & 0x01;
  if (is_empty == false) {
    return;
  }

  uint8_t data = 0;
  BaseType_t is_recv = xQueueReceiveFromISR(
      interrupt_config->tx_queue, (void *)&data, &pxHigherPriorityTaskWoken);
  if (is_recv) {
    usart->THR = data;
    uart_interrupt__callback_notify(interrupt_config,
                                    UART_interrupt_event_TX_COMPLETE);
  } else {
    // end the transmission with an empty frame
    usart->THR = 0;
    uart_interrupt__callback_notify(interrupt_config,
                                    UART_interrupt_event_TRANSFER_COMPLETE);
    usart->IER &= ~((1 << IER_THREIE));
  }

  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}

static void uart_interrupt__handle_rda(UART_interrupt_s *interrupt_config) {
  BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
  USART_TypeDef *usart = interrupt_config->uart_config->usart;
  uint8_t data = usart->RBR;
  xQueueSendFromISR(interrupt_config->rx_queue, (void *)&data,
                    &pxHigherPriorityTaskWoken);
  uart_interrupt__callback_notify(interrupt_config,
                                  UART_interrupt_event_RX_READY);
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}

static void uart_interrupt__handle_errors(UART_interrupt_s *interrupt_config) {
  USART_TypeDef *usart = interrupt_config->uart_config->usart;

  uint32_t lsr_data = usart->LSR;
  uint32_t errors = (lsr_data >> 1) & 0xF;

  switch (errors) {
  case 0x1:
    uart_interrupt__callback_notify(interrupt_config,
                                    UART_interrupt_event_RX_OVERFLOW);
    break;
  case 0x2:
    uart_interrupt__callback_notify(interrupt_config,
                                    UART_interrupt_event_RX_PARITY_ERROR);
    break;
  case 0x4:
    uart_interrupt__callback_notify(interrupt_config,
                                    UART_interrupt_event_RX_FRAMING_ERROR);
    break;
  case 0x8:
    uart_interrupt__callback_notify(interrupt_config,
                                    UART_interrupt_event_BREAK);
    break;
  default:
    // Catch all the other errors here
    break;
  case 0:
    // No errors
    break;
  }
}

static void uart_interrupt__callback_notify(UART_interrupt_s *interrupt_config,
                                            UART_interrupt_event_e event) {
  if (interrupt_config->UART_event_cb != NULL) {
    interrupt_config->UART_event_cb(event);
  }
}
