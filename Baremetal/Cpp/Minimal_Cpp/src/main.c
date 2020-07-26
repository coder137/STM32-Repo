#include "stm32l475xx.h"
#include <stdint.h>

static void init();
static void set();
static void reset();
static void spin_delay(uint32_t delay);

int main(void) {
  init();
  set();
  spin_delay(1000 * 1000);

  while (1) {
    reset();
    spin_delay(1000 * 1000);

    set();
    spin_delay(1000 * 1000);
  }

  return 0;
}

static void init() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  GPIOA->BRR |= (1 << 5); // Reset the pin here

  // Set the mode
  GPIOA->MODER &= ~(3 << 10);
  GPIOA->MODER |= (1 << 10); // 01 00 00 00 00 00

  // Check these registers
  GPIOA->OTYPER &= ~(1 << 5); // set to 0
  GPIOA->OSPEEDR &= ~(3 << 10);
  GPIOA->PUPDR &= ~(3 << 10);
}

static void set() {
  // Set the pin here
  GPIOA->BSRR |= (1 << 5);
}

static void reset() {
  GPIOA->BRR = (1 << 5); // Reset
}

static void spin_delay(uint32_t delay) {
  while (delay) {
    __NOP();
    --delay;
  }
}
