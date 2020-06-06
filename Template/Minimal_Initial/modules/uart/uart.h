#ifndef MODULES_UART_H
#define MODULES_UART_H

#include <stdint.h>

#include "stm32l4xx.h"

typedef enum {
  UART_word_length_8,
  UART_word_length_9,
  UART_word_length_7,
} UART_word_length_e;

typedef enum {
  UART_stop_bit_1_0,
  UART_stop_bit_0_5,
  UART_stop_bit_2_0,
  UART_stop_bit_1_5,
} UART_stop_bit_e;

// Main Struct
typedef struct {
  USART_TypeDef *usart;

  UART_word_length_e word_length;
  UART_stop_bit_e stop_bit;
  uint32_t baud_rate;
} UART_s;

void uart__init(UART_s *config, USART_TypeDef *usart);

void uart__write(const UART_s *config, const char data);
void uart__write_string(const UART_s *config, const char *buffer);

#endif
