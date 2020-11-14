#include "uart.h"

#include <stdint.h>

// Constants
static const uint8_t CR1_UE = 0;
static const uint8_t CR1_RE = 2;

static const uint8_t CR1_PS = 9;
static const uint8_t CR1_PCE = 10;

static const uint8_t CR1_M0 = 12;
static const uint8_t CR1_OVER8 = 15;
static const uint8_t CR1_M1 = 28;

static const uint8_t CR2_STOP = 12;

// static const uint8_t ISR_ORE = 3;
static const uint8_t ISR_RXNE = 5;
static const uint8_t ISR_TC = 6;
static const uint8_t ISR_TXE = 7;

// STATIC FUNCTION DECLARATIONS
static void uart__disable(UART_s *config);
static void uart__enable(UART_s *config);

static void uart__set_word_length(UART_s *config);
static void uart__set_parity(UART_s *config);
static void uart__set_stop_bit(UART_s *config);
static void uart__set_baud_rate(UART_s *config);

static void uart__set_communication_mode(UART_s *config);
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

  // Define the Parity Bit in CR1
  uart__set_parity(config);

  // Define the Baud Rate in BRR Register
  uart__set_baud_rate(config);

  // Number of stop bits in CR2
  uart__set_stop_bit(config);

  // Activate TE Bit
  uart__set_communication_mode(config);

  // Enable the USART CR1 (UE Bit)
  uart__enable(config);
}

void uart__deinit(UART_s *config) { uart__disable(config); }

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
  while ((usart->ISR & (1 << ISR_RXNE)) == 0) {
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
  bool not_implemented = false;
  uint32_t wordlength = 0;
  switch (config->word_length) {
  case UART_word_length_8:
    wordlength = 0;
    break;
  case UART_word_length_9:
    wordlength = 1;
    break;
  case UART_word_length_7:
    wordlength = 2;
    break;

  case UART_word_length_5:
  case UART_word_length_6:
  default:
    not_implemented = true;
    break;
  }

  if (!not_implemented) {
    const uint8_t m1 = (wordlength >> 1) & 0x01;
    const uint8_t m0 = wordlength & 0x01;

    // Clear and set
    uint32_t cr1_data = config->usart->CR1;
    cr1_data &= ~((1 << CR1_M1) | (1 << CR1_M0));
    cr1_data |= ((m1 << CR1_M1) | (m0 << CR1_M0));
    config->usart->CR1 = cr1_data;
  }
}

static void uart__set_parity(UART_s *config) {
  uint8_t p_enable = 0;
  uint8_t p_value = 0;
  switch (config->parity) {
  case UART_parity_NONE:
    p_enable = 0;
    break;
  case UART_parity_EVEN:
    p_enable = 1;
    p_value = 0;
    break;
  case UART_parity_ODD:
    p_enable = 1;
    p_value = 1;
    break;
  default:
    // keep defaults as above
    break;
  }

  uint32_t cr1_data = config->usart->CR1;
  cr1_data &= ~((1 << CR1_PCE) | (1 << CR1_PS));
  cr1_data |= ((p_enable << CR1_PCE) | (p_value << CR1_PS));
  config->usart->CR1 = cr1_data;
}

static void uart__set_stop_bit(UART_s *config) {
  uint8_t stop_bit = 0;
  switch (config->stop_bit) {
  case UART_stop_bit_1_0:
    stop_bit = 0;
    break;
  case UART_stop_bit_0_5:
    stop_bit = 1;
    break;
  case UART_stop_bit_2_0:
    stop_bit = 2;
    break;
  case UART_stop_bit_1_5:
    stop_bit = 3;
    break;
  default:
    break;
  }

  uint32_t cr2_data = config->usart->CR2;
  cr2_data &= ~(3 << CR2_STOP);
  cr2_data |= (stop_bit << CR2_STOP);
  config->usart->CR2 = cr2_data;
}

// Formula on Page 1348
static void uart__set_baud_rate(UART_s *config) {
  USART_TypeDef *usart = config->usart;

  // Set OVER8 to 0 (Oversampling by 16)
  usart->CR1 &= ~(1 << CR1_OVER8);

  const uint32_t usartdiv = (SystemCoreClock / config->baud_rate);
  usart->BRR = (usartdiv);
}

static void uart__set_communication_mode(UART_s *config) {
  uint8_t communication_mode = 0;
  switch (config->communication_mode) {
  case UART_communication_mode_INACTIVE:
    communication_mode = 0;
    break;
  case UART_communication_mode_RX_ONLY:
    communication_mode = 1;
    break;
  case UART_communication_mode_TX_ONLY:
    communication_mode = 2;
    break;
  case UART_communication_mode_RX_TX:
    communication_mode = 3;
    break;
  default:
    break;
  }

  uint32_t cr1_data = config->usart->CR1;
  cr1_data &= ~(3 << CR1_RE);
  cr1_data |= (communication_mode << CR1_RE);
  config->usart->CR1 = cr1_data;
}
