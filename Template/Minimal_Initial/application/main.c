#include <stdint.h>

#include "gpio/gpio.h"

static void _spin_delay(uint32_t delay);

int main(void) {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

  GPIO_s config = {};
  config.mode = GPIO_mode_OUTPUT;
  config.type = GPIO_type_PUSH_PULL;
  config.speed = GPIO_speed_LOW_SPEED;
  config.pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;

  gpio__init(&config, GPIOA, 5);
  gpio__set(&config);

  while (1) {
    _spin_delay(1000 * 1000);
    gpio__reset(&config);

    _spin_delay(1000 * 1000);
    gpio__set(&config);
  }

  return 0;
}

static void _spin_delay(uint32_t delay) {
  while (delay) {
    __NOP();
    --delay;
  }
}
