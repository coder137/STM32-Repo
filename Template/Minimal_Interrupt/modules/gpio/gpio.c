#include "gpio.h"

// HELPER function
static void gpio__set_byte_2(uint32_t *port_register, uint8_t pin,
                             uint8_t value) {
  *port_register |= (value << (pin << 1));
}
static void gpio__reset_byte_2(uint32_t *port_register, uint8_t pin) {
  *port_register &= ~(3 << (pin << 1));
}
static void gpio__set_byte_1(uint32_t *port_register, uint8_t pin,
                             uint8_t value) {
  *port_register |= (value << pin);
}
static void gpio__reset_byte_1(uint32_t *port_register, uint8_t pin) {
  *port_register &= ~(1 << pin);
}

// STATIC FUNCTION DECLARATIONS
static void gpio__update_moder(const GPIO_s *config);
static void gpio__update_otyper(const GPIO_s *config);
static void gpio__update_ospeedr(const GPIO_s *config);
static void gpio__update_pupdr(const GPIO_s *config);
static void gpio__update_afr(const GPIO_s *config);

// TODO, Add more here

// FUNCTION
void gpio__init(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin) {
  // Set Port and Pin
  config->port = port;
  config->pin = pin;

  // Set MODER
  gpio__update_moder(config);

  // Set TYPE
  gpio__update_otyper(config);

  // Set SPEED
  gpio__update_ospeedr(config);

  // Set PULL
  gpio__update_pupdr(config);

  // Set AFR
  gpio__update_afr(config);
}

void gpio__set(const GPIO_s *config) {
  uint32_t bsrr_data = config->port->BSRR;
  gpio__set_byte_1(&bsrr_data, config->pin, 1);
  config->port->BSRR = bsrr_data;
}

void gpio__reset(const GPIO_s *config) {
  uint32_t brr_data = config->port->BRR;
  gpio__set_byte_1(&brr_data, config->pin, 1);
  config->port->BRR = brr_data;
}

bool gpio__get(const GPIO_s *config) {
  bool ispressed = false;
  if (config->mode == GPIO_mode_INPUT) {
    ispressed = (config->port->IDR >> config->pin) & 0x01;
  }
  return ispressed;
}

// STATIC FUNCTION DEFINITIONS
static void gpio__update_moder(const GPIO_s *config) {
  uint32_t moder_data = config->port->MODER;
  gpio__reset_byte_2(&moder_data, config->pin);
  gpio__set_byte_2(&moder_data, config->pin, config->mode);
  config->port->MODER = moder_data;
}

static void gpio__update_otyper(const GPIO_s *config) {
  uint32_t otyper_data = config->port->OTYPER;
  gpio__reset_byte_1(&otyper_data, config->pin);
  gpio__set_byte_1(&otyper_data, config->pin, config->type);
  config->port->OTYPER = otyper_data;
}

static void gpio__update_ospeedr(const GPIO_s *config) {
  uint32_t ospeedr_data = config->port->OSPEEDR;
  gpio__reset_byte_2(&ospeedr_data, config->pin);
  gpio__set_byte_2(&ospeedr_data, config->pin, config->speed);
  config->port->OSPEEDR = ospeedr_data;
}

static void gpio__update_pupdr(const GPIO_s *config) {
  uint32_t pupdr_data = config->port->PUPDR;
  gpio__reset_byte_2(&pupdr_data, config->pin);
  gpio__set_byte_2(&pupdr_data, config->pin, config->pull);
  config->port->PUPDR = pupdr_data;
}

static void gpio__update_afr(const GPIO_s *config) {
  if (config->mode == GPIO_mode_ALTERNATE_FUNCTION) {
    // 0000 - 0111 -> Index 0
    // 1000 - 1111 -> Index 1
    const uint8_t index = config->pin >> 3;

    // 0 - 8
    // 1 - 9
    // .. etc
    const uint8_t shift = (config->pin % 8) << 2;

    // Compute
    uint32_t afr_data = config->port->AFR[index];
    afr_data &= ~(0xF << shift);
    afr_data |= (config->alternate << shift);

    // Write to memory only once
    config->port->AFR[index] = afr_data;
  }
}
