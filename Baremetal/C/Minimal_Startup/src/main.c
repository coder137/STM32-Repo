#include <stdint.h>

#include "stm32l475xx.h"

static void _spin_delay(uint32_t delay);

static uint32_t bss_value;
static uint32_t data_value = 0x11223344;  // check this in the debug window
int main(void) {
  (void)bss_value;
  (void)data_value;

  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  GPIOA->BRR |= (1 << 5);  // Reset the pin here

  // Set the mode
  GPIOA->MODER &= ~(3 << 10);
  GPIOA->MODER |= (1 << 10);  // 01 00 00 00 00 00

  // Check these registers
  GPIOA->OTYPER &= ~(1 << 5);  // set to 0
  GPIOA->OSPEEDR &= ~(3 << 10);
  GPIOA->PUPDR &= ~(3 << 10);

  // Set the pin here
  GPIOA->BSRR |= (1 << 5);

  while (1) {
    _spin_delay(1000 * 1000);
    GPIOA->BRR = (1 << 5);  // Reset

    _spin_delay(1000 * 1000);
    GPIOA->BSRR = (1 << 5);  // Set
  }

  return 0;
}

static void _spin_delay(uint32_t delay) {
  while (delay) {
    __NOP();
    --delay;
  }
}
