#include <stdint.h>

#include "gpio.h"

#include "uart/uart.h"
#include "uart/uart_interrupt.h"

// CONSTANTS
static const uint32_t USART1_RX_BUF_SIZE = 100;
static const uint32_t USART1_TX_BUF_SIZE = 100;
static const uint32_t USART1_TX_PIN = 6;
static const uint32_t USART1_RX_PIN = 7;
static const uint32_t USART1_BAUD_RATE = 115200;

// STATIC DECLARATION
// TODO, Shift this to a different file when more peripherals are added
static void system_uart__init();
static void system_uart_interrupt__init();

// STATE VARIABLES
static UART_s uart_config;

// CAN BE EXTERNED
// Called by syscalls.c
UART_interrupt_s uart_interrupt_config;

// FUNCTION
void peripheral__initialize(void) {
  // Initialize UART for System
  system_uart__init();
  system_uart_interrupt__init();

  // TODO, Initialize more systems here
}

// STATIC FUNCTION
static void system_uart__init() {

  // Activate USART1
  RCC->APB2ENR |= (1 << 14);
  // Activate GPIOB
  RCC->AHB2ENR |= (1 << 1);

  GPIO_s config = {};
  config.mode = GPIO_mode_ALTERNATE_FUNCTION;
  config.type = GPIO_type_PUSH_PULL;
  config._internal.speed = GPIO_speed_VERY_HIGH_SPEED;
  config._internal.pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  config._internal.alternate = GPIO_alternate_7;
  gpio__init(&config, GPIOB, USART1_TX_PIN);
  gpio__init(&config, GPIOB, USART1_RX_PIN);

  uart_config.baud_rate = USART1_BAUD_RATE;
  uart_config.stop_bit = UART_stop_bit_1_0;
  uart_config.word_length = UART_word_length_8;
  uart_config.mode = UART_mode_RX_TX;
  uart__init(&uart_config, USART1);
}

static void system_uart_interrupt__init() {
  uart_interrupt_config.usart = uart_config.usart;
  uart_interrupt_config.rx_queue_length = USART1_RX_BUF_SIZE;
  uart_interrupt_config.tx_queue_length = USART1_TX_BUF_SIZE;
  uart_interrupt__init(&uart_interrupt_config);
}

// INTERRUPT FUNCTION
void USART1_Handler(void) { uart_interrupt__process(&uart_interrupt_config); }
