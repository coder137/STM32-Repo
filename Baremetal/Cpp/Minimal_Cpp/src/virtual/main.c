/**
 * NOTE: Read the `virtual/main.cpp` code comments first
 *
 * - Virtual C++ with Classes with args (with constructor) = 1228 (40 bytes
 * more)
 * - Virtual C++ with Classes with args with link time optimization (with
 * constructor) = 1212 (24 bytes more than general example) i.e 16 bytes saved
 * from above example
 *
 * - C with function pointer to struct (similar to virtual class) i.e 1212 bytes
 * (Similar code size to virtual C++ class example with link time optimization)
 *
 * NOTE: C and C++ can generate strictly similar code output with little to no
 * code bloat
 *
 */
#include "stm32l475xx.h"
#include <stdint.h>

struct Blink {
  GPIO_TypeDef *port;
  uint8_t pin;

  void (*init)(struct Blink *blink);
  void (*set)(struct Blink *blink);
  void (*reset)(struct Blink *blink);
  void (*spin_delay)(struct Blink *blink, uint32_t delay);
};

void init(struct Blink *blink);
void set(struct Blink *blink);
void reset(struct Blink *blink);
void spin_delay(struct Blink *blink, uint32_t delay);

int main(void) {
  struct Blink blink = {
      .port = GPIOA,
      .pin = 5,
      .init = init,
      .set = set,
      .reset = reset,
      .spin_delay = spin_delay,
  };

  blink.init(&blink);
  blink.set(&blink);
  blink.spin_delay(&blink, 1000 * 1000);

  while (1) {
    blink.reset(&blink);
    blink.spin_delay(&blink, 1000 * 1000);

    blink.set(&blink);
    blink.spin_delay(&blink, 1000 * 1000);
  }

  return 0;
}

void init(struct Blink *blink) {
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

void set(struct Blink *blink) {
  // Set the pin here
  blink->port->BSRR |= (1 << blink->pin);
}

void reset(struct Blink *blink) {
  blink->port->BRR = (1 << blink->pin); // Reset
}

void spin_delay(struct Blink *blink, uint32_t delay) {
  (void)blink;

  while (delay) {
    __NOP();
    --delay;
  }
}
