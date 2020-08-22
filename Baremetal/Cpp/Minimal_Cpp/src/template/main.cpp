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
 * C with Struct with args = 1188
 * C++ with Classes with args (with constructor) = 1188
 * Virtual C++ with Classes with args (with constructor) = 1228 (40 bytes more)
 * C++ with Templates and move constructor = 1188
 *
 */
#include "stm32l475xx.h"

#include <cstdint>
#include <utility>

// Machine specific blink
// Similar to a class implementation for a virtual class
class Blink {
public:
  Blink(GPIO_TypeDef *port, std::uint8_t pin);

  void init();
  void set();
  void reset();
  void spin_delay(std::uint32_t delay);

private:
  GPIO_TypeDef *port;
  std::uint8_t pin;
};

// Template blink used by the sections
// Similar to a virtual class
// Better than virtual class -> Compile time optimized
// * Be careful against template explosion, avoid using many machine
// specific implementations in code
// * Typically these interfaces should only have one implementation thus
// generating very optimized code without duplication
template <typename MachineBlink = Blink> class TemplateBlink {
public:
  // Since this is meant to be an example for a low level driver, we remove this
  // For higher level layers, it would be better to pass in data by reference
  TemplateBlink(MachineBlink &b) = delete;
  TemplateBlink(MachineBlink &&b) : b_(b) {}
  void init() { b_.init(); }
  void set() { b_.set(); }
  void reset() { b_.reset(); }
  void spin_delay(std::uint32_t delay) { b_.spin_delay(delay); }

private:
  MachineBlink b_;
};

Blink::Blink(GPIO_TypeDef *port, std::uint8_t pin) : port(port), pin(pin) {}

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
  TemplateBlink blink{Blink{GPIOA, 5}};

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
