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

typedef enum {
  UART_mode_INACTIVE,
  UART_mode_RX_ONLY,
  UART_mode_TX_ONLY,
  UART_mode_RX_TX
} UART_mode_e;

// Main Struct
typedef struct {
  USART_TypeDef *usart;

  UART_word_length_e word_length;
  UART_stop_bit_e stop_bit;
  UART_mode_e mode;
  uint32_t baud_rate;
} UART_s;

void uart__init(UART_s *config, USART_TypeDef *usart);

void uart__write(const UART_s *config, const char data);
void uart__write_string(const UART_s *config, const char *buffer);

// ! Error could take place in this function
// * OE (Overflow Error)
// TODO, Update this with Interrupts (store data into a buffer when data is
// recvd) OR
// TODO, Return an error value as well from this function
uint8_t uart__read(const UART_s *config);

#endif
