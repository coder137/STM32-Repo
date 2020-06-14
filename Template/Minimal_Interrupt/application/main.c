#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "exti/exti.h"
#include "gpio/gpio.h"
#include "uart/uart.h"

/**
 * STATIC FUNCTION DECLARATIONS
 */

static void main__uart_init(void);
static void main__uart_interrupt_init(void);

static void main__gpio_output(void);

static void main__gpio_input(void);
static void main__gpio_input_external_interrupt(void);

static void _spin_delay(uint32_t delay);

/**
 * STATE VARIABLES
 */
// GPIO OUTPUT
static GPIO_s output_config;

// GPIO INPUT
static GPIO_s input_config;
static EXTI_s gpioC13_interrupt_config;
volatile bool is_button_pressed = false;

// UART
static UART_s uart_config;

// UART RX
#define RX_BUF_SIZE 255
volatile bool is_newline = false;
static uint8_t rx_buf[RX_BUF_SIZE];
static uint32_t counter;

void EXTI15_10_Handler(void) {

  if (exti__gpio_is_pending_interrupt(&gpioC13_interrupt_config)) {
    exti__gpio_clear_pending_interrupt(&gpioC13_interrupt_config);

    // Your logic here
    is_button_pressed = true;
  }
}

// TODO, Modify this to handle both RX and TX Interrupts
// TODO, Handle Errors with this Interrupt system
// TODO, Update this with FreeRTOS Queue system
void USART1_Handler(void) {
  uint8_t value = uart__read_from_interrupt(&uart_config);
  rx_buf[counter] = value;
  counter++;

  if (value == 0x0d || value == 0x0a) {
    is_newline = true;
  }
}

int main(void) {
  // UART Config
  main__uart_init();
  main__uart_interrupt_init();
  uart__write_string(&uart_config, "Hello World\r\n");

  // GPIO Update
  main__gpio_output();
  main__gpio_input();
  main__gpio_input_external_interrupt();

  while (1) {

    if (is_button_pressed) {
      gpio__set(&output_config);
      _spin_delay(1000 * 1000);
      gpio__reset(&output_config);
      is_button_pressed = false;
    }

    if (is_newline) {
      uart__write_string(&uart_config, "Printing\r\n");

      // Print to console
      for (int i = 0; i < counter; i++) {
        char buf[15] = {0};
        sprintf(buf, "%d : %x %c\r\n", i, rx_buf[i], rx_buf[i]);
        uart__write_string(&uart_config, buf);
      }

      // Reset Values
      memset(rx_buf, 0, sizeof(uint8_t) * RX_BUF_SIZE);
      counter = 0;
      is_newline = false;
    }
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

static void main__uart_interrupt_init(void) {
  uart__interrupt_init(&uart_config);
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

static void main__gpio_input_external_interrupt(void) {

  RCC->APB2ENR |= (1 << 0);

  // 0-3
  // 4-7
  // 8-11
  // 12-15
  SYSCFG->EXTICR[3] |= ((1 << 1) << 4); // Set the 13th bit

  // Connected to External Pullup resistor
  gpioC13_interrupt_config.type = EXTI_type_FALLING;
  gpioC13_interrupt_config.pin = input_config.pin;
  exti__gpio_register_interrupt(&gpioC13_interrupt_config);

  NVIC_EnableIRQ(EXTI15_10_IRQn);
}
