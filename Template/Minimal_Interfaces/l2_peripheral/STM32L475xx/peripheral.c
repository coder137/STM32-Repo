#include <stdint.h>

#include "gpio.h"
#include "uart.h"
#include "uart_interrupt.h"

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
static UART_interrupt_s uart_interrupt_config;

// FUNCTION
void peripheral__initialize(void) {
  // Initialize UART for System
  system_uart__init();
  system_uart_interrupt__init();

  // TODO, Initialize more systems here
}

// Currently used for debugging
static void uart_interrupt_cb(UART_interrupt_event_e event) {
  switch (event) {
  case UART_interrupt_event_TX_COMPLETE:
    __NOP();
    break;
  case UART_interrupt_event_SEND_FRAME_COMPLETE:
    __NOP();
    break;
  case UART_interrupt_event_TRANSFER_COMPLETE:
    __NOP();
    break;
  default:
    __NOP();
    break;
  }
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
  config.pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  config.alternate = GPIO_alternate_7;

  // TODO, Shift to a function
  config._internal.speed = GPIO_speed_VERY_HIGH_SPEED;
  gpio__init(&config, GPIOB, USART1_TX_PIN);
  gpio__init(&config, GPIOB, USART1_RX_PIN);

  uart_config.baud_rate = USART1_BAUD_RATE;
  uart_config.stop_bit = UART_stop_bit_1_0;
  uart_config.word_length = UART_word_length_8;
  uart_config.parity = UART_parity_NONE;
  uart_config.communication_mode = UART_communication_mode_RX_TX;
  uart__init(&uart_config, USART1);
}

static void system_uart_interrupt__init() {
  uart_interrupt_config.uart_config = &uart_config;
  uart_interrupt_config.rx_queue_length = USART1_RX_BUF_SIZE;
  uart_interrupt_config.tx_queue_length = USART1_TX_BUF_SIZE;
  uart_interrupt_config.UART_event_cb = uart_interrupt_cb;
  uart_interrupt__init(&uart_interrupt_config);

  // This is for FreeRTOS kernel APIs to be used within ISR.
  // ISR priority is set to be lower that the FreeRTOS kernel (higher number) so
  // that we can call the FreeRTOS ISR functions
  NVIC_SetPriority(USART1_IRQn,
                   configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
  NVIC_EnableIRQ(USART1_IRQn);
}

// Interrupt function
void USART1_Handler(void) { uart_interrupt__process(&uart_interrupt_config); }

// Newlib Overrides
int _read(int file, char *ptr, int len) {
  *ptr = uart_interrupt__read(&uart_interrupt_config, portMAX_DELAY);
  return 1;
}

int _write(int file, char *ptr, int len) {
  uart_interrupt__write_string_n(&uart_interrupt_config, ptr, len);
  return len;
}
