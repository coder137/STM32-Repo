#include <stdint.h>

#include "gpio/gpio.h"
#include "uart/uart.h"

// State Variables
static GPIO_s output_config;
static GPIO_s input_config;
static UART_s uart_config;

// Static Functions
static void main__uart_init(void);
static void main__gpio_output(void);
static void main__gpio_input(void);
static void _spin_delay(uint32_t delay);

int main(void) {
  // GPIO Update
  main__gpio_output();
  main__gpio_input();

  // UART Config
  main__uart_init();
  uart__write_string(&uart_config, "Hello World\r\n");

  while (1) {
    _spin_delay(1000 * 1000);
    gpio__reset(&output_config);

    // Test gpio input
    bool ispressed = gpio__get(&input_config);
    if (ispressed) {
      uart__write_string(&uart_config, "Not Pressed\r\n");
    } else {
      uart__write_string(&uart_config, "Pressed\r\n");
    }

    // Test uart__read
    // ! OE Error if you write more than one character here
    // TODO, Solve using interrupts
    uart__write_string(&uart_config, "Waiting for one character\r\n");
    char value = (char)uart__read(&uart_config);
    uart__write_string(&uart_config, "Read Value: ");
    uart__write(&uart_config, value);
    uart__write(&uart_config, '\r');
    uart__write(&uart_config, '\n');

    _spin_delay(1000 * 1000);
    gpio__set(&output_config);
  }

  return 0;
}

// STATIC FUNCTION
static void main__uart_init(void) {
  // Activate USART1
  RCC->APB2ENR |= (1 << 14);
  // Activate GPIOB
  RCC->AHB2ENR |= (1 << 1);

  GPIO_s config = {};
  config.mode = GPIO_mode_ALTERNATE_FUNCTION;
  config.type = GPIO_type_PUSH_PULL;
  config.speed = GPIO_speed_VERY_HIGH_SPEED;
  config.pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  config.alternate = GPIO_alternate_7;
  gpio__init(&config, GPIOB, 6);
  gpio__init(&config, GPIOB, 7);

  uart_config.baud_rate = 115200U;
  uart_config.stop_bit = UART_stop_bit_1_0;
  uart_config.word_length = UART_word_length_8;
  uart_config.mode = UART_mode_RX_TX;
  uart__init(&uart_config, USART1);
}

static void main__gpio_output(void) {
  // Activate GPIOA
  RCC->AHB2ENR |= (1 << 0);

  output_config.mode = GPIO_mode_OUTPUT;
  output_config.type = GPIO_type_PUSH_PULL;
  output_config.speed = GPIO_speed_LOW_SPEED;
  output_config.pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  gpio__init(&output_config, GPIOA, 5);
  gpio__set(&output_config);
}

static void main__gpio_input(void) {
  // Activate GPIOC
  RCC->AHB2ENR |= (1 << 2);

  input_config.mode = GPIO_mode_INPUT;
  input_config.type = GPIO_type_PUSH_PULL;
  input_config.speed = GPIO_speed_LOW_SPEED;
  input_config.pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  gpio__init(&input_config, GPIOC, 13);
}

static void _spin_delay(uint32_t delay) {
  while (delay) {
    __NOP();
    --delay;
  }
}
