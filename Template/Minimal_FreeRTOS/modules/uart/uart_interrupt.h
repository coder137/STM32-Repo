#ifndef MODULES_UART_INTERRUPT_H
#define MODULES_UART_INTERRUPT_H

#include <stdint.h>

#include "stm32l4xx.h"

#include "FreeRTOS.h"
#include "queue.h"

typedef struct {
  QueueHandle_t rx_queue;
  QueueHandle_t tx_queue;
} UART_interrupt_internal_s;

typedef struct {
  USART_TypeDef *usart;
  uint32_t rx_queue_length;
  uint32_t tx_queue_length;

  // Do not update this
  UART_interrupt_internal_s _internal;
} UART_interrupt_s;

void uart_interrupt__init(UART_interrupt_s *interrupt_config);

void uart_interrupt__write(UART_interrupt_s *interrupt_config, uint8_t data);
void uart_interrupt__write_string(UART_interrupt_s *interrupt_config,
                                  char *data);

uint8_t uart_interrupt__read(const UART_interrupt_s *interrupt_config);

// Used by Interrupt Function
void uart_interrupt__process(const UART_interrupt_s *interrupt_config);

#endif
