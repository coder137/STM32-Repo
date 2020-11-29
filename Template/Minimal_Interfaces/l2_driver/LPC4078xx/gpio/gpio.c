#include "gpio.h"

static void gpio__update_mode(GPIO_s *config);
static void gpio__update_type(GPIO_s *config);
static void gpio__update_pull(GPIO_s *config);
static void gpio__update_alternate_function(GPIO_s *config);

void gpio__init(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin) {
  config->port = port;
  config->pin = pin;

  gpio__update_mode(config);
  gpio__update_type(config);
  gpio__update_pull(config);
  gpio__update_alternate_function(config);
}

void gpio__write(const GPIO_s *config, uint8_t value) {
  if (value) {
    gpio__set(config);
  } else {
    gpio__reset(config);
  }
}

void gpio__set(const GPIO_s *config) {
  config->port->SET |= (1 << config->pin);
}

void gpio__reset(const GPIO_s *config) {
  config->port->CLR |= (1 << config->pin);
}

bool gpio__read(const GPIO_s *config) {
  return (config->port->PIN & (1 << config->pin));
}

// High level APIs
void gpio__init_as_output(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin) {}
void gpio__init_as_input(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin) {}

// Static functions
static void gpio__update_mode(GPIO_s *config) {

  switch (config->mode) {
  case GPIO_mode_INPUT:
    config->port->DIR &= ~(1 << config->pin);
    break;
  case GPIO_mode_OUTPUT:
    config->port->DIR |= (1 << config->pin);
    break;
  default:
    config->port->DIR &= ~(1 << config->pin);
    break;
  }
}

// TODO, Make the driver more generic
static void gpio__update_type(GPIO_s *config) {}
static void gpio__update_pull(GPIO_s *config) {}
static void gpio__update_alternate_function(GPIO_s *config) {}
