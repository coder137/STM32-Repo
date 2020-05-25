#include "stm32l475xx.h"
#include <stdint.h>

static void init(void);
static void reset(void);
static void set(void);

static void _spin_delay(uint32_t delay);

int main(void) {
  init();
  set();
  while (1) {
    _spin_delay(1000 * 1000);
    reset();

    _spin_delay(1000 * 1000);
    set();
  }

  return 0;
}

static void init(void) {
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

static void reset(void) {
  GPIOA->BRR = (1 << 5); // Reset
}

static void set(void) {
  // Set the pin here
  GPIOA->BSRR |= (1 << 5);
}

static void _spin_delay(uint32_t delay) {
  while (delay) {
    __NOP();
    --delay;
  }
}
