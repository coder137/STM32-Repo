#include "uart.h"

// Static functions

// Variables
static UART_s uconfig;

void peripheral__initialize() {
  GPIO_s gpio_u_config = {
      .alternate = GPIO_alternate_1,
  };
  gpio__init(&gpio_u_config, LPC_GPIO0, 2);
  gpio__init(&gpio_u_config, LPC_GPIO0, 3);

  uconfig.baud_rate = 115200;
  uconfig.communication_mode = UART_communication_mode_RX_TX;
  uconfig.parity = UART_parity_NONE;
  uconfig.stop_bit = UART_stop_bit_1_0;
  uconfig.word_length = UART_word_length_8;
  uart__init(&uconfig, LPC_UART0);
}

// Syscalls
int _write(int file, char *ptr, int len) {
  for (int i = 0; i < len; i++) {
    uart__write(&uconfig, ptr[i]);
  }
  return len;
}

int _read(int file, char *ptr, int len) {
  *ptr = uart__read(&uconfig);
  return 1;
}
