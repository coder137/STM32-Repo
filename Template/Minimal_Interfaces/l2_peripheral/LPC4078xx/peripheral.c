#include "uart.h"
#include "uart_interrupt.h"

// Constants
static const uint32_t UART0_RX_BUF_SIZE = 100;
static const uint32_t UART0_TX_BUF_SIZE = 100;
// static const uint32_t UART0_TX_PIN = 6;
// static const uint32_t UART0_RX_PIN = 7;
static const uint32_t UART0_BAUD_RATE = 115200;

// Static functions
static void system_uart__init();
static void system_uart_interrupt__init();

// Variables
UART_s uart_config;
static UART_interrupt_s uart_interrupt_config;

void peripheral__initialize() {
  system_uart__init();
  system_uart_interrupt__init();
}

static void system_uart__init() {
  GPIO_s gpio_u_config = {
      .alternate = GPIO_alternate_1,
  };
  // TODO, Replace with constants
  gpio__init(&gpio_u_config, LPC_GPIO0, 2);
  gpio__init(&gpio_u_config, LPC_GPIO0, 3);

  uart_config.baud_rate = UART0_BAUD_RATE;
  uart_config.communication_mode = UART_communication_mode_RX_TX;
  uart_config.parity = UART_parity_NONE;
  uart_config.stop_bit = UART_stop_bit_1_0;
  uart_config.word_length = UART_word_length_8;
  uart__init(&uart_config, LPC_UART0);
}

// TODO, Unused
static void UART_event_cb(UART_interrupt_event_e event) {
  switch (event) {
  case UART_interrupt_event_TX_COMPLETE:
    break;
  case UART_interrupt_event_SEND_FRAME_COMPLETE:
    break;
  case UART_interrupt_event_TRANSFER_COMPLETE:
    break;
  default:
    break;
  }
}

static void system_uart_interrupt__init() {
  uart_interrupt_config.uart_config = &uart_config;
  uart_interrupt_config.rx_queue_length = UART0_RX_BUF_SIZE;
  uart_interrupt_config.tx_queue_length = UART0_TX_BUF_SIZE;
  uart_interrupt_config.UART_event_cb = UART_event_cb;

  uart_interrupt__init(&uart_interrupt_config);

  // Necessary to use FreeRTOS Interrupt APIs
  NVIC_SetPriority(UART0_IRQn,
                   configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
  NVIC_EnableIRQ(UART0_IRQn);
}

// UART0 Interrupt
void UART0_Handler(void) { uart_interrupt__process(&uart_interrupt_config); }

// Syscalls
int _write(int file, char *ptr, int len) {
  uart_interrupt__write_string_n(&uart_interrupt_config, ptr, len);
  return len;
}

int _read(int file, char *ptr, int len) {
  // TODO, Write this code
  *ptr = uart_interrupt__read(&uart_interrupt_config, 1000);
  return 1;
}
