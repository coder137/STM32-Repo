#include "stm32l475xx.h"
#include <stdint.h>

class Blink {
public:
  Blink();

  void init();
  void set();
  void reset();
  void spin_delay(uint32_t delay);
};

int main(void) {
  Blink b;
  b.init();

  b.set();
  b.spin_delay(1000 * 1000);

  while (1) {
    b.reset();
    b.spin_delay(1000 * 1000);
    b.set();
    b.spin_delay(1000 * 1000);
  }

  return 0;
}

Blink::Blink() {}

void Blink::init() {
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

void Blink::set() {
  GPIOA->BSRR |= (1 << 5); // Set
}

void Blink::reset() {
  GPIOA->BRR = (1 << 5); // Reset
}

void Blink::spin_delay(uint32_t delay) {
  while (delay) {
    __NOP();
    --delay;
  }
}
