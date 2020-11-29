
#include "gpio.h"
#include "uart.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

extern int main();

void _start(void) {
  // l5_application layer
  main();

  while (1) {
  }
}

// ! Added for Testing
// ! Shift to l5_application layer later
// 96_000_000 cycles per second
// 96_000 cycles per millisecond
// 96 cycles per microsecond
static void delay_us(uint32_t us);
static void delay_ms(uint32_t ms);
static UART_s uconfig;

static void print_task(void *arg) {
  uint32_t counter = 0;
  while (1) {
    printf("Hello World %ld\r\n", counter++);
    vTaskDelay(100);
  }
}

static void blink_task(void *arg) {
  GPIO_s config;
  config.mode = GPIO_mode_OUTPUT;
  gpio__init(&config, LPC_GPIO1, 18);
  while (1) {
    gpio__set(&config);
    vTaskDelay(100);
    gpio__reset(&config);
    vTaskDelay(100);
  }
}

int main() {
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

  xTaskCreate(print_task, "print_task", 2048, NULL, 2, NULL);
  xTaskCreate(blink_task, "blink_task", 2048, NULL, 2, NULL);
  vTaskStartScheduler();

  uint32_t counter = 0;
  while (1) {
    printf("Hello World %ld\r\n", counter++);
    delay_ms(100);
    // gpio__reset(&config);
    delay_ms(100);
  }

  return 0;
}

static void delay_us(uint32_t us) {
  for (int i = 0; i < (96 * us); i++) {
    __NOP();
  }
}

static void delay_ms(uint32_t ms) {
  for (int i = 0; i < ms; i++) {
    delay_us(1000);
  }
}

int _write(int file, char *ptr, int len) {
  for (int i = 0; i < len; i++) {
    uart__write(&uconfig, ptr[i]);
  }
  return len;
}
