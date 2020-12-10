#include "gpio.h"
#include "uart.h"

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

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

static void read_task(void *arg) {
  char buffer[20] = {0};
  while (1) {
    scanf("%s", buffer);
    printf("Received: %s\r\n", buffer);
    memset(buffer, 0, sizeof(buffer));
  }
}

// Overriding the default GPIO_Handler
volatile bool is_pressed = false;
void GPIO_Handler(void) {
  is_pressed = true;

  // Clear pin 29
  LPC_GPIOINT->IO0IntClr |= (1 << 29);
}

static void gpio_interrupt(void *arg) {
  // Configure the interrupt
  GPIO_s iconfig;
  gpio__init_as_input(&iconfig, LPC_GPIO0, 29);
  LPC_GPIOINT->IO0IntEnR |= (1 << iconfig.pin);

  NVIC_EnableIRQ(GPIO_IRQn);

  // Check for the interrupt here
  while (1) {
    if (is_pressed) {
      printf("Pressed\r\n");
      is_pressed = false;
    } else {
      vTaskDelay(10);
    }
  }
}

int main() {

  xTaskCreate(print_task, "print_task", 2048, NULL, 2, NULL);
  xTaskCreate(blink_task, "blink_task", 2048, NULL, 2, NULL);
  xTaskCreate(gpio_interrupt, "gpio_interrupt", 2048, NULL, 2, NULL);
  xTaskCreate(read_task, "read_task", 2048, NULL, 2, NULL);
  vTaskStartScheduler();

  return 0;
}
