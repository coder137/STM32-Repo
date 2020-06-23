#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "exti/exti.h"
#include "gpio/gpio.h"

#include "uart/uart.h"
#include "uart/uart_interrupt.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/**
 * STATIC FUNCTION DECLARATIONS
 */

void main__uart_init(void);
void main__uart_interrupt_init(void);

void main__gpio_output(void);

void main__gpio_input(void);
void main__gpio_input_external_interrupt(void);

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
UART_s uart_config;
static UART_interrupt_s uart_interrupt_config;
volatile bool is_tc = false;

// UART RX
#define RX_BUF_SIZE 255
#define TX_BUF_SIZE 255

// MISC
static QueueHandle_t qHandle;

// INTERRUPTS
void EXTI15_10_Handler(void) {
  if (exti__gpio_is_pending_interrupt(&gpioC13_interrupt_config)) {
    exti__gpio_clear_pending_interrupt(&gpioC13_interrupt_config);

    // Your logic here
    is_button_pressed = true;
  }
}

void USART1_Handler(void) { uart_interrupt__process(&uart_interrupt_config); }

static void USART1_Transmit_Complete_Cb(void) { is_tc = true; }

// TASKS
void uart_task(void *arg) {

  // uart_interrupt__write_string(&uart_interrupt_config,
  //                              "Hello from Interrupt\r\n");
  uart__write_string(&uart_config, "Hello Interrupt\r\n");

  uint8_t data = 0;
  char buf[20] = {0};

  while (1) {
    // uart_interrupt__write_string(&uart_interrupt_config, "ON\r\n");
    // vTaskDelay(1000);
    // uart_interrupt__write_string(&uart_interrupt_config, "OFF\r\n");
    // vTaskDelay(1000);
    xQueueReceive(qHandle, &data, portMAX_DELAY);
    memset(buf, 0, 20 * sizeof(char));
    sprintf(buf, "%x %d\r\n", data, data);

    // uart_interrupt__write_string(&uart_interrupt_config, buf);
    uart__write_string(&uart_config, buf);
    vTaskDelay(1000);
  }
}

void sender_task(void *arg) {
  uint8_t counter = 0;
  while (1) {
    xQueueSend(qHandle, &counter, 0);
    counter++;
    vTaskDelay(1000);
  }
}

void uart_receiver(void *arg) {
  uart_interrupt__write_string(&uart_interrupt_config,
                               "Hello uart_receiver\r\n");

  char buf[20] = {0};

  while (1) {
    uint8_t data = uart_interrupt__read(&uart_interrupt_config);

    memset(buf, 0, 20 * sizeof(char));
    sprintf(buf, "%x %c\r\n", data, data);
    uart_interrupt__write_string(&uart_interrupt_config, buf);
  }
}

void cb_checker(void *arg) {
  while (1) {
    if (is_tc) {
      is_tc = false;

      // Run
      gpio__set(&output_config);
      vTaskDelay(100);
      gpio__reset(&output_config);
    }
  }
}

void blink_task(void *arg) {
  while (1) {
    gpio__set(&output_config);
    vTaskDelay(1000);
    gpio__reset(&output_config);
    vTaskDelay(1000);
  }
}

int main(void) {
  main__uart_init();
  main__uart_interrupt_init();

  main__gpio_output();
  gpio__reset(&output_config);

  qHandle = xQueueCreate(100, sizeof(uint8_t));

  // xTaskCreate(uart_task, "uart_task", 2000, NULL, 1, NULL);
  // xTaskCreate(sender_task, "sender", 2000, NULL, 1, NULL);
  xTaskCreate(uart_receiver, "uart_recevier", 2000, NULL, 1, NULL);
  xTaskCreate(cb_checker, "tc_checker", 2000, NULL, 1, NULL);
  // xTaskCreate(blink_task, "blink", 2000, NULL, 1, NULL);
  vTaskStartScheduler();

  // vTaskStartSchedular should never exit
  while (1) {
  }

  return 0;
}

// STATIC FUNCTION
void main__uart_init(void) {
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

void main__uart_interrupt_init(void) {
  uart_interrupt_config.usart = uart_config.usart;
  uart_interrupt_config.rx_queue_length = RX_BUF_SIZE;
  uart_interrupt_config.tx_queue_length = TX_BUF_SIZE;
  uart_interrupt_config.UART_transmit_complete_cb = USART1_Transmit_Complete_Cb;
  uart_interrupt__init(&uart_interrupt_config);
}

void main__gpio_output(void) {
  // Activate GPIOA
  RCC->AHB2ENR |= (1 << 0);

  output_config.mode = GPIO_mode_OUTPUT;
  output_config.type = GPIO_type_PUSH_PULL;
  output_config.speed = GPIO_speed_LOW_SPEED;
  output_config.pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  gpio__init(&output_config, GPIOA, 5);
  gpio__set(&output_config);
}

void main__gpio_input(void) {
  // Activate GPIOC
  RCC->AHB2ENR |= (1 << 2);

  input_config.mode = GPIO_mode_INPUT;
  input_config.type = GPIO_type_PUSH_PULL;
  input_config.speed = GPIO_speed_LOW_SPEED;
  input_config.pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  gpio__init(&input_config, GPIOC, 13);
}

void main__gpio_input_external_interrupt(void) {

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
