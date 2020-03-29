void __main(void);

__attribute__((section(".isr_vector"))) const void (*isr_vector[240])(void) = {
    (void *)0x20000400, (void *)__main,
    // TODO, Add more here
};

// COMMON section
int global_uninit;
const int const_uninit;

// data section
int global_init = 10;
static int static_init = 10;

// bss section
static int static_uninit;
static const int const_static_uninit;

// rodata section
const int const_init = 10;
static const int const_static_init = 10;

#include <stdint.h>

#include "stm32l475xx.h"

static void _spin_delay(uint32_t delay);

void __main(void) {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  GPIOA->BRR |= (1 << 5); // Reset the pin here

  // Set the mode
  GPIOA->MODER &= ~(3 << 10);
  GPIOA->MODER |= (1 << 10); // 01 00 00 00 00 00

  // Check these registers
  GPIOA->OTYPER &= ~(1 << 5); // set to 0
  GPIOA->OSPEEDR &= ~(3 << 10);
  GPIOA->PUPDR &= ~(3 << 10);

  // Set the pin here
  GPIOA->BSRR |= (1 << 5);

  while (1) {
    _spin_delay(1000 * 1000);
    GPIOA->BRR = (1 << 5); // Reset

    _spin_delay(1000 * 1000);
    GPIOA->BSRR = (1 << 5); // Set
  }
}

static void _spin_delay(uint32_t delay) {
  while (delay) {
    __NOP();
    --delay;
  }
}
