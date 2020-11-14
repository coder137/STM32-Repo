#include <stdint.h>
#include <stdio.h>
#include <string.h>

// From Driver
#include "exti/exti.h"
#include "gpio.h"
#include "rcc/rcc.h"

#include "uart.h"
#include "uart_interrupt.h"

// From HAL
#include "input_gpio/input_gpio.h"
#include "output_gpio/output_gpio.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/**
 * STATIC FUNCTION DECLARATIONS
 */
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

// MISC

// INTERRUPTS
void EXTI15_10_Handler(void) {
  if (exti__gpio_is_pending_interrupt(&gpioC13_interrupt_config)) {
    exti__gpio_clear_pending_interrupt(&gpioC13_interrupt_config);

    // Your logic here
    is_button_pressed = true;
  }
}

// TASKS
static void blink_task(void *arg) {
  while (1) {
    gpio__set(&output_config);
    vTaskDelay(1000);
    gpio__reset(&output_config);
    vTaskDelay(1000);
  }
}

static void uart_write_task(void *arg) {
  uint32_t counter = 0;
  while (1) {
    printf("Hello World: %ld\r\n", counter);
    counter++;
    vTaskDelay(1000);
  }
}

static void uart_read_task(void *arg) {
  char buf[20] = {0};
  uint32_t counter = 0;
  while (1) {
    scanf("%s", buf);
    printf("Recv %ld: %s\r\n", counter, buf);
    counter++;
  }
}

static void gpio_input_task(void *arg) {
  while (1) {
    // HIGH is not pressed
    // LOW is pressed
    if (gpio__read(&input_config) == false) {
      printf("Pressed\r\n");
    }
    vTaskDelay(200);
  }
}

int main(void) {
  printf("Main\r\n");
  output_gpio__init(&output_config, GPIOA, 5, RCC_AHB2ENR_GPIOAEN);
  input_gpio__init(&input_config, GPIOC, 13, RCC_AHB2ENR_GPIOCEN);
  gpio__reset(&output_config);

  // GPIO Tests
  xTaskCreate(blink_task, "blink led test", 1000, NULL, 1, NULL);
  xTaskCreate(gpio_input_task, "gpio input test", 1000, NULL, 1, NULL);

  // UART Tests
  xTaskCreate(uart_write_task, "printf test", 2000, NULL, 2, NULL);
  xTaskCreate(uart_read_task, "scanf test", 2000, NULL, 2, NULL);

  // Start the FreeRTOS scheduler
  vTaskStartScheduler();

  // vTaskStartSchedular should never exit
  while (1) {
  }

  return 0;
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
