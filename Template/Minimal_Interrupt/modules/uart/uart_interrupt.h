#ifndef MODULES_UART_INTERRUPT_H
#define MODULES_UART_INTERRUPT_H

#include <stdint.h>

#include "stm32l4xx.h"

typedef struct {
  USART_TypeDef *usart;

  uint8_t *rx_buffer;
  uint32_t rx_buffer_length;
  uint32_t rx_buffer_count;

  uint8_t *tx_buffer;
  uint32_t tx_buffer_length;
  uint32_t tx_buffer_count;
} UART_interrupt_s;

void uart_interrupt__init(UART_interrupt_s *interrupt_config);

void uart_interrupt__write_string(UART_interrupt_s *interrupt_config,
                                  char *data);

void uart_interrupt__process(const UART_interrupt_s *interrupt_config);

#endif
