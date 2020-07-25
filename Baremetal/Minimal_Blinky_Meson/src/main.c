#include <stdint.h>

#include "stm32l475xx.h"

void __main(void);

__attribute__((section(".isr_vector"))) const void (*isr_vector[240])(void) = {
    (void *)0x20000400, (void *)__main,
    // TODO, Add more here
};

// COMMON section
uint32_t global_uninit;
const uint8_t const_uninit;

// data section
uint32_t global_init = 10;
uint8_t global_init_1 = 10;
// missing
static uint8_t static_init = 10;

// bss section
uint32_t bss_init_0 = 0;
uint8_t bss_init_0_1 = 0;
static uint32_t static_uninit;
static const uint8_t const_static_uninit;

// rodata section
const uint32_t const_init = 10;
const uint8_t const_init_1 = 10;

// missing
static const uint8_t const_static_init = 10;

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
