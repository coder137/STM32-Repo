/**
 *
 * Having a constructor (and destructor) increases the binary size by 4 bytes
 * over its C counter part
 * However code generation for both C and C++ strictly results in the same
 * binary output if (constructor and destructors arent used)
 *
 * C with Struct without args = 1152
 * C++ with Classes without args (without constructor) = 1152
 * C++ with Classes without args (with constructor) = 1156
 *
 * - C with Struct with args = 1188
 * - C++ with Classes with args (with constructor) = 1188
 * - C++ with Classes with args with link time optimization (with constructor) =
 * 1108 (80 bytes lesser than C version!)
 * - Virtual C++ with Classes with args (with constructor) = 1228 (40 bytes
 * more)
 * - Virtual C++ with Classes with args with link time optimization (with
 * constructor) = 1212 (24 bytes more than general example) i.e 16 bytes saved
 * from above example
 *
 * NOTE: Removing specialized constructor reduces the code size by 12-16 bytes
 * OR,
 * NOTE: Make the specialized constructor `constexpr`
 * However virtual classes are still around 30-40 bytes larger than their
 * non-virtual class counter parts
 *
 */
#include "stm32l475xx.h"
#include <cstdint>

// Abstract class
class VBlink {
public:
  virtual void init() = 0;
  virtual void set() = 0;
  virtual void reset() = 0;
  virtual void spin_delay(std::uint32_t delay) = 0;
};

// Machine specific class
class Blink final : public VBlink {
public:
  constexpr Blink(GPIO_TypeDef *port, std::uint8_t pin);

  void init() override;
  void set() override;
  void reset() override;
  void spin_delay(std::uint32_t delay) override;

private:
  GPIO_TypeDef *port;
  std::uint8_t pin;
};

constexpr Blink::Blink(GPIO_TypeDef *port, std::uint8_t pin)
    : port(port), pin(pin) {}

void Blink::init() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  this->port->BRR |= (1 << this->pin); // Reset the pin here

  // Set the mode
  this->port->MODER &= ~(3 << (this->pin * 2));
  this->port->MODER |= (1 << (this->pin * 2)); // 01 00 00 00 00 00

  // Check these registers
  this->port->OTYPER &= ~(1 << this->pin); // set to 0
  this->port->OSPEEDR &= ~(3 << (this->pin * 2));
  this->port->PUPDR &= ~(3 << (this->pin * 2));
}

void Blink::set() {
  // Set the pin here
  this->port->BSRR |= (1 << this->pin);
}

void Blink::reset() {
  this->port->BRR = (1 << this->pin); // Reset
}

void Blink::spin_delay(std::uint32_t delay) {
  while (delay) {
    __NOP();
    --delay;
  }
}

int main() {
  Blink blink{GPIOA, 5};

  blink.init();
  blink.set();
  blink.spin_delay(1000 * 1000);

  while (1) {
    blink.reset();
    blink.spin_delay(1000 * 1000);

    blink.set();
    blink.spin_delay(1000 * 1000);
  }

  return 0;
}
