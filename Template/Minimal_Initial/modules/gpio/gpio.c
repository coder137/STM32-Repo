#include "gpio.h"

// HELPER function
static void gpio__set_byte_2(volatile uint32_t *port_register, uint8_t pin,
                             uint8_t value) {
  *port_register |= (value << (pin << 1));
}
static void gpio__reset_byte_2(volatile uint32_t *port_register, uint8_t pin) {
  *port_register &= ~(3 << (pin << 1));
}
static void gpio__set_byte_1(volatile uint32_t *port_register, uint8_t pin,
                             uint8_t value) {
  *port_register |= (value << pin);
}
static void gpio__reset_byte_1(volatile uint32_t *port_register, uint8_t pin) {
  *port_register &= ~(1 << pin);
}

// STATIC FUNCTION DECLARATIONS
static void gpio__set_moder(const GPIO_s *config);
static void gpio__reset_moder(const GPIO_s *config);

static void gpio__set_otyper(const GPIO_s *config);
static void gpio__reset_otyper(const GPIO_s *config);

static void gpio__set_ospeedr(const GPIO_s *config);
static void gpio__reset_ospeedr(const GPIO_s *config);

static void gpio__set_pupdr(const GPIO_s *config);
static void gpio__reset_pupdr(const GPIO_s *config);

static void gpio__update_afr(const GPIO_s *config);

// TODO, Add more here

// FUNCTION
void gpio__init(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin) {
  // Set Port and Pin
  config->port = port;
  config->pin = pin;

  // Set MODER
  gpio__reset_moder(config);
  gpio__set_moder(config);

  // Set TYPE
  gpio__reset_otyper(config);
  gpio__set_otyper(config);

  // Set SPEED
  gpio__reset_ospeedr(config);
  gpio__set_ospeedr(config);

  // Set PULL
  gpio__reset_pupdr(config);
  gpio__set_pupdr(config);

  // Set Alternate
  gpio__update_afr(config);
}

void gpio__set(const GPIO_s *config) {
  gpio__set_byte_1(&(config->port->BSRR), config->pin, 1);
}

void gpio__reset(const GPIO_s *config) {
  gpio__set_byte_1(&(config->port->BRR), config->pin, 1);
}

// STATIC FUNCTION DEFINITIONS
static void gpio__set_moder(const GPIO_s *config) {
  gpio__set_byte_2(&(config->port->MODER), config->pin, config->mode);
}

static void gpio__reset_moder(const GPIO_s *config) {
  gpio__reset_byte_2(&(config->port->MODER), config->pin);
}

static void gpio__set_otyper(const GPIO_s *config) {
  gpio__set_byte_1(&(config->port->OTYPER), config->pin, config->type);
}

static void gpio__reset_otyper(const GPIO_s *config) {
  gpio__reset_byte_1(&(config->port->OTYPER), config->pin);
}

static void gpio__set_ospeedr(const GPIO_s *config) {
  gpio__set_byte_2(&(config->port->OSPEEDR), config->pin, config->speed);
}

static void gpio__reset_ospeedr(const GPIO_s *config) {
  gpio__reset_byte_2(&(config->port->OSPEEDR), config->pin);
}

static void gpio__set_pupdr(const GPIO_s *config) {
  gpio__set_byte_2(&(config->port->PUPDR), config->pin, config->pull);
}
static void gpio__reset_pupdr(const GPIO_s *config) {
  gpio__reset_byte_2(&(config->port->PUPDR), config->pin);
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
