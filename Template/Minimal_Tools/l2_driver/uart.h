#ifndef DRIVER_INTERFACE_USART_H_
#define DRIVER_INTERFACE_USART_H_

#include <stdint.h>

#include "gpio.h"

// platform specific
#include "uart/uart_internal.h"

// https://github.com/ARM-software/CMSIS_5/blob/develop/CMSIS/Driver/Include/Driver_USART.h

typedef enum {
  UART_word_length_8,
  UART_word_length_9,
  UART_word_length_7,
  UART_word_length_5,
  UART_word_length_6,
} UART_word_length_e;

typedef enum {
  UART_stop_bit_1_0,
  UART_stop_bit_0_5,
  UART_stop_bit_2_0,
  UART_stop_bit_1_5,
} UART_stop_bit_e;

typedef enum {
  UART_communication_mode_INACTIVE,
  UART_communication_mode_RX_ONLY,
  UART_communication_mode_TX_ONLY,
  UART_communication_mode_RX_TX
} UART_communication_mode_e;

typedef enum {
  UART_parity_NONE,
  UART_parity_EVEN,
  UART_parity_ODD,
} UART_parity_e;

// Main Struct
typedef struct {
  USART_TypeDef *usart;

  UART_communication_mode_e communication_mode;
  UART_word_length_e word_length;
  UART_stop_bit_e stop_bit;
  UART_parity_e parity;

  uint32_t baud_rate;
} UART_s;

void uart__init(UART_s *config, USART_TypeDef *usart);
void uart__deinit(UART_s *config);

// NOTE, These APIS Read and Write directly to the register in a polling manner.
void uart__write(const UART_s *config, const char data);
void uart__write_string(const UART_s *config, const char *buffer);

// ! Error could take place in this function
// * OE (Overflow Error) when writing data very fast
uint8_t uart__read(const UART_s *config);

#endif // DRIVER_UART_UART_H_
