#include <stdint.h>
#include <stdio.h>
#include <string.h>

// From Driver
#include "exti/exti.h"
#include "gpio/gpio.h"
#include "rcc/rcc.h"

#include "uart/uart.h"
#include "uart/uart_interrupt.h"

// From HAL
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
void blink_task(void *arg) {
  while (1) {
    printf("Hello %s\r\n", __FUNCTION__);
    gpio__set(&output_config);
    vTaskDelay(1000);
    gpio__reset(&output_config);
    vTaskDelay(1000);
  }
}

void uart_read(void *arg) {
  char buf[20] = {0};
  while (1) {
    printf("Waiting for data>\r\n");
    scanf("%s", buf);
    printf("Data: %s\r\n", buf);
  }
}

int main(void) {
  printf("Starting main\r\n");
  output_gpio__init(&output_config, GPIOA, 5, RCC_AHB2ENR_GPIOAEN);
  gpio__reset(&output_config);

  xTaskCreate(blink_task, "printf test", 2000, NULL, 1, NULL);
  xTaskCreate(uart_read, "scanf test", 2000, NULL, 2, NULL);
  vTaskStartScheduler();

  // vTaskStartSchedular should never exit
  while (1) {
  }

  return 0;
}

void main__gpio_input(void) {
  // Activate GPIOC
  rcc__set_ahb2_peripheral_clock_enable(RCC_AHB2ENR_GPIOCEN);

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
