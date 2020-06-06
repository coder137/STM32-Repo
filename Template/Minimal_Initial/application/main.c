#include <stdint.h>

#include "gpio/gpio.h"
#include "uart/uart.h"

static void _spin_delay(uint32_t delay);

int main(void) {

  // Can activate into a single line
  // Activate USART1
  RCC->APB2ENR |= (1 << 14);

  // Activate GPIOA
  RCC->AHB2ENR |= (1 << 0);
  // Activate GPIOB
  RCC->AHB2ENR |= (1 << 1);

  // UART Pin Selection
  GPIO_s config = {};
  config.mode = GPIO_mode_ALTERNATE_FUNCTION;
  config.type = GPIO_type_PUSH_PULL;
  config.speed = GPIO_speed_VERY_HIGH_SPEED;
  config.pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  config.alternate = GPIO_alternate_7;
  gpio__init(&config, GPIOB, 6);
  gpio__init(&config, GPIOB, 7);

  // GPIO Update
  config.mode = GPIO_mode_OUTPUT;
  config.speed = GPIO_speed_LOW_SPEED;

  gpio__init(&config, GPIOA, 5);
  gpio__set(&config);

  // UART Config
  UART_s uart_config = {};
  uart_config.baud_rate = 115200U;
  uart_config.stop_bit = UART_stop_bit_1_0;
  uart_config.word_length = UART_word_length_8;
  uart__init(&uart_config, USART1);

  while (1) {
    _spin_delay(1000 * 1000);
    gpio__reset(&config);
    uart__write_string(&uart_config, "Hello World\r\n");

    _spin_delay(1000 * 1000);
    gpio__set(&config);
    uart__write(&uart_config, 'I');
    uart__write(&uart_config, '\r');
    uart__write(&uart_config, '\n');
  }

  return 0;
}

static void _spin_delay(uint32_t delay) {
  while (delay) {
    __NOP();
    --delay;
  }
}
