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

// FUNCTION
void uart_interrupt__init(UART_interrupt_s *interrupt_config) {
  // RX Interrupt
  interrupt_config->usart->CR1 |= ((1 << CR1_RXINE));

  NVIC_EnableIRQ(USART1_IRQn);
}

void uart_interrupt__write(const UART_interrupt_s *interrupt_config,
                           uint8_t data) {
  // Add this to the uart_queue
  interrupt_config->usart->TDR = data;
}

void uart_interrupt__write_string(UART_interrupt_s *interrupt_config,
                                  char *data) {

  USART_TypeDef *usart = interrupt_config->usart;
  uint32_t cr1_data = usart->CR1;
  uint8_t txe_enabled = (cr1_data >> CR1_TXEIE) & 0x01;
  uint8_t tc_enabled = (cr1_data >> CR1_TCIE) & 0x01;
  uint8_t combined = (txe_enabled << 1) | tc_enabled;

  if (txe_enabled == 0) {
    // Reset the value
    memset(interrupt_config->tx_buffer, 0,
           sizeof(uint8_t) * interrupt_config->tx_buffer_length);
    interrupt_config->tx_buffer_count = 0;

    // Copy the data
    strncpy((char *)interrupt_config->tx_buffer, data,
            interrupt_config->tx_buffer_length);

    // Activate the data
    usart->CR1 |= ((1 << CR1_TXEIE) | (1 << CR1_TCIE));
  } else {
    // Append the data to the buffer
    strncat((char *)interrupt_config->tx_buffer, data,
            interrupt_config->tx_buffer_length);
  }

  return;

  switch (combined) {
    // Both are OFF
  case 0:
    // Copy the data
    strncpy((char *)interrupt_config->tx_buffer, data,
            interrupt_config->tx_buffer_length);
    // Reactivate the data
    cr1_data |= ((1 << CR1_TXEIE) | (1 << CR1_TCIE));
    usart->CR1 = cr1_data;
    break;
    // TXE just turned off, TC is still ON
  case 1:
    // Copy the data
    memset(interrupt_config->tx_buffer, 0, interrupt_config->tx_buffer_length);
    strncpy((char *)interrupt_config->tx_buffer, data,
            interrupt_config->tx_buffer_length);

    // Memset and reactivate just cr1_data
    cr1_data |= ((1 << CR1_TXEIE));
    usart->CR1 = cr1_data;
    break;
    // Both are active
  case 3:
    // strcat
    strncat((char *)interrupt_config->tx_buffer, data,
            interrupt_config->tx_buffer_length);
    break;
  // NOTE, Should not happen since we will never deactivate tc before txe
  case 2:
  default:
    break;
  }
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
  // DONE, Add this to a RX Queue
  interrupt_config->rx_buffer[interrupt_config->rx_buffer_count] =
      (uint8_t)(interrupt_config->usart->RDR);
  interrupt_config->rx_buffer_count++;

  // char buf[15] = {0};
  // sprintf(buf, "i:%lx\r\n", uart_interrupt_config.usart->ISR);
  // uart_i_write(buf);

  // if (value == 0x0d || value == 0x0a) {
  //   is_newline = true;
  // }
}

static void uart_interrupt_process_txe(UART_interrupt_s *interrupt_config) {
  if (interrupt_config->tx_buffer[interrupt_config->tx_buffer_count] != 0) {
    interrupt_config->usart->TDR =
        interrupt_config->tx_buffer[interrupt_config->tx_buffer_count];
    interrupt_config->tx_buffer_count++;
  } else {
    // Will stay at 1 (but will no longer cause an interrupt)
    interrupt_config->usart->CR1 &= ~(1 << CR1_TXEIE);
  }
}

static void uart_interrupt__process_tc(UART_interrupt_s *interrupt_config) {
  interrupt_config->tx_buffer_count = 0;
  memset(interrupt_config->tx_buffer, 0,
         interrupt_config->tx_buffer_length * sizeof(uint8_t));

  interrupt_config->usart->CR1 &= ~(1 << CR1_TCIE);
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
