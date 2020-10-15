#include "stm32l475xx.h"
#include <stdint.h>

#include <bit>

namespace blink {

static void init() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

  uint32_t pin = 5; // will be passed as a class parameter
  const uint32_t left_shift_1_5 = std::rotl(static_cast<uint32_t>(1), pin);
  const uint32_t left_shift_1_10 = std::rotl(static_cast<uint32_t>(1), pin * 2);
  const uint32_t left_shift_3_10 = std::rotl(static_cast<uint32_t>(3), pin * 2);

  GPIOA->BRR |= left_shift_1_5; // Reset the pin here

  // Set the mode
  uint32_t moder_data = GPIOA->MODER;
  moder_data &= ~left_shift_3_10;
  moder_data |= left_shift_1_10; // 01 00 00 00 00 00
  GPIOA->MODER = moder_data;

  // Check these registers
  GPIOA->OTYPER &= ~left_shift_1_5; // set to 0
  GPIOA->OSPEEDR &= ~left_shift_3_10;
  GPIOA->PUPDR &= ~left_shift_3_10;
}

static void set() {
  constexpr uint32_t left_shift_1_5 = std::rotl(static_cast<uint32_t>(1), 5);
  GPIOA->BSRR |= left_shift_1_5; // Set
}

static void reset() {
  constexpr uint32_t left_shift_1_5 = std::rotl(static_cast<uint32_t>(1), 5);
  GPIOA->BRR = left_shift_1_5; // Reset
}

static void spin_delay(uint32_t delay) {
  while (delay) {
    __NOP();
    --delay;
  }
}

} // namespace blink

int main() {
  blink::init();
  blink::set();
  blink::spin_delay(1000 * 1000);

  while (1) {
    blink::reset();
    blink::spin_delay(1000 * 1000);

    blink::set();
    blink::spin_delay(1000 * 1000);
  }

  return 0;
}
