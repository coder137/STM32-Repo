#include "stm32l475xx.h"
#include <stdint.h>

typedef struct {
  GPIO_TypeDef *port;
  uint8_t pin;
} Blink;

void init(Blink *blink);
void set(Blink *blink);
void reset(Blink *blink);
void spin_delay(Blink *blink, uint32_t delay);

int main(void) {
  Blink blink = {GPIOA, 5};

  init(&blink);
  set(&blink);
  spin_delay(&blink, 1000 * 1000);

  while (1) {
    reset(&blink);
    spin_delay(&blink, 1000 * 1000);

    set(&blink);
    spin_delay(&blink, 1000 * 1000);
  }

  return 0;
}

void init(Blink *blink) {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  blink->port->BRR |= (1 << blink->pin); // Reset the pin here

  // Set the mode
  blink->port->MODER &= ~(3 << (blink->pin * 2));
  blink->port->MODER |= (1 << (blink->pin * 2)); // 01 00 00 00 00 00

  // Check these registers
  blink->port->OTYPER &= ~(1 << blink->pin); // set to 0
  blink->port->OSPEEDR &= ~(3 << (blink->pin * 2));
  blink->port->PUPDR &= ~(3 << (blink->pin * 2));
}

void set(Blink *blink) {
  // Set the pin here
  blink->port->BSRR |= (1 << blink->pin);
}

void reset(Blink *blink) {
  blink->port->BRR = (1 << blink->pin); // Reset
}

void spin_delay(Blink *blink, uint32_t delay) {
  (void)blink;

  while (delay) {
    __NOP();
    --delay;
  }
}
