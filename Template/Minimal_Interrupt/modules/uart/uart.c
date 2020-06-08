#include "uart.h"

#include <stdint.h>

#include "stm32l4xx.h"

// Constants
static const uint8_t CR1_UE = 0;
static const uint8_t CR1_RE = 2;
static const uint8_t CR1_M0 = 12;
static const uint8_t CR1_OVER8 = 15;
static const uint8_t CR1_M1 = 28;

static const uint8_t CR2_STOP = 12;

static const uint8_t ISR_TC = 6;
static const uint8_t ISR_TXE = 7;

// STATIC FUNCTION DECLARATIONS
static void uart__disable(UART_s *config);
static void uart__enable(UART_s *config);

static void uart__set_word_length(UART_s *config);
static void uart__set_stop_bit(UART_s *config);
static void uart__set_baud_rate(UART_s *config);

static void uart__set_mode(UART_s *config);
// Section 40.5.2 -> UART Transmitter
// Page 1340

// Section 40.5.3 -> UART Receiver
void uart__init(UART_s *config, USART_TypeDef *usart) {
  // Set the usart here
  config->usart = usart;

  // Disable the UART first
  uart__disable(config);

  // TODO, Disable the other stuff here
  // USART1->CR2 &= ~((1 << 14) | (1 << 11));
  // USART1->CR3 &= ~((1 << 5) | (1 << 3) | (1 << 1));

  // Define the Word Length in CR1
  uart__set_word_length(config);

  // TODO, Disable Parity control
  // USART1->CR1 &= ~(1 << 10);

  // Define the Baud Rate in BRR Register
  uart__set_baud_rate(config);

  // Number of stop bits in CR2
  uart__set_stop_bit(config);

  // Enable the USART CR1 (UE Bit)
  uart__enable(config);

  // Activate TE Bit
  uart__set_mode(config);
}

void uart__write(const UART_s *config, const char data) {
  USART_TypeDef *usart = config->usart;

  // Wait till USART1->ISR bit 7 is not 0
  while ((usart->ISR & (1 << ISR_TXE)) == 0) {
  }

  // Write data to send in TDR register
  usart->TDR = data;

  while ((usart->ISR & (1 << ISR_TC)) == 0) {
  }
}

void uart__write_string(const UART_s *config, const char *buffer) {
  USART_TypeDef *usart = config->usart;

  while (*buffer != 0) {
    while ((usart->ISR & (1 << ISR_TXE)) == 0) {
    }

    usart->TDR = *buffer;
    buffer++;
  }

  while ((usart->ISR & (1 << ISR_TC)) == 0) {
  }
}

uint8_t uart__read(const UART_s *config) {
  USART_TypeDef *usart = config->usart;

  // Wait for RXNE bit to be set
  while ((usart->ISR & (1 << 5)) == 0) {
  }

  uint8_t data = usart->RDR;
  return data;
}

// STATIC FUNCTION DECLARATION

// Clear the UE pin
static void uart__disable(UART_s *config) {
  config->usart->CR1 &= ~(1 << CR1_UE);
}

// Set the UE pin
static void uart__enable(UART_s *config) {
  config->usart->CR1 |= (1 << CR1_UE);
}

// Clear M0 and M1, Set M0 and M1 later (dynamically)
static void uart__set_word_length(UART_s *config) {
  USART_TypeDef *usart = config->usart;
  const uint8_t m1 = (config->word_length >> 1) & 0x01;
  const uint8_t m0 = config->word_length & 0x01;

  // Clear and set
  usart->CR1 &= ~((1 << CR1_M1) | (1 << CR1_M0));
  usart->CR1 |= ((m1 << CR1_M1) | (m0 << CR1_M0));
}

// Set the STOP Register
static void uart__set_stop_bit(UART_s *config) {
  config->usart->CR2 &= ~(3 << CR2_STOP);
  config->usart->CR2 |= (config->stop_bit << CR2_STOP);
}

// Formula on Page 1348
static void uart__set_baud_rate(UART_s *config) {
  USART_TypeDef *usart = config->usart;

  // Set OVER8 to 0 (Oversampling by 16)
  usart->CR1 &= ~(1 << CR1_OVER8);

  const uint32_t usartdiv = (SystemCoreClock / config->baud_rate);
  usart->BRR = (usartdiv);
}

static void uart__set_mode(UART_s *config) {
  uint32_t cr1_data = config->usart->CR1;
  cr1_data &= ~(3 << CR1_RE);
  cr1_data |= (config->mode << CR1_RE);
  config->usart->CR1 = cr1_data;
}
