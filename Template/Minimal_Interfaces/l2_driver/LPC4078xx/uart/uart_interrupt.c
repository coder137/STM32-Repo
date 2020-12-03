#include "uart_interrupt.h"

#include "gpio.h"
#include "uart.h"

#include <stdio.h>

// Constants

// static const uint8_t IER_RBRIE = 0;
static const uint8_t IER_THREIE = 1;
// static const uint8_t IER_RXIE = 2;

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
static void uart_interrupt__callback_notify(UART_interrupt_s *interrupt_config,
                                            UART_interrupt_event_e event);

void uart_interrupt__init(UART_interrupt_s *interrupt_config) {
  interrupt_config->rx_queue =
      xQueueCreate(interrupt_config->rx_queue_length, sizeof(uint8_t));
  interrupt_config->tx_queue =
      xQueueCreate(interrupt_config->tx_queue_length, sizeof(uint8_t));

  // TODO, Activate RX Interrupts here
  // USART_TypeDef *usart = interrupt_config->uart_config->usart;
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

// TODO, Incomplete
uint8_t uart_interrupt__read(const UART_interrupt_s *interrupt_config,
                             uint32_t wait_for) {
  return 0;
}

// TODO, Incomplete
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
    break;
  case 0x3:
    // RLS (Receive Line Status)
    break;
  case 0x6:
    // CTI (Character Timeout Indicator)
    break;
  default:
    break;
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
  USART_TypeDef *usart = interrupt_config->uart_config->usart;
  uint32_t lsr_data = usart->LSR;
  bool is_empty = (lsr_data >> LSR_THRE) & 0x01;
  if (is_empty == false) {
    return;
  }

  uint8_t data = 0;
  BaseType_t is_recv =
      xQueueReceiveFromISR(interrupt_config->tx_queue, (void *)&data, NULL);
  if (is_recv) {
    usart->THR = data;
    uart_interrupt__callback_notify(interrupt_config,
                                    UART_interrupt_event_TX_COMPLETE);
  } else {
    usart->IER &= ~((1 << IER_THREIE));
    usart->THR = 0;
    uart_interrupt__callback_notify(interrupt_config,
                                    UART_interrupt_event_TRANSFER_COMPLETE);
  }
}

static void uart_interrupt__callback_notify(UART_interrupt_s *interrupt_config,
                                            UART_interrupt_event_e event) {
  if (interrupt_config->UART_event_cb != NULL) {
    interrupt_config->UART_event_cb(event);
  }
}
