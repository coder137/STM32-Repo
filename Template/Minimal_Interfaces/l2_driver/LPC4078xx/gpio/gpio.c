#include "gpio.h"
#include <stddef.h>

static void gpio__update_mode(GPIO_s *config);
static void gpio__update_type(GPIO_s *config);
static void gpio__update_pull(GPIO_s *config);
static void gpio__update_alternate_function(GPIO_s *config);

// HELPER STATICS
static volatile uint32_t *gpio__get_iocon(GPIO_s *config);

// VARIABLES
static const uint8_t IOCON_FUNC = 0;
static const uint8_t IOCON_MODE = 3;
static const uint8_t IOCON_OD = 10;

// FUNCTIONS
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
void gpio__init_as_output(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin) {
  config->mode = GPIO_mode_OUTPUT;
  config->type = GPIO_type_PUSH_PULL;
  config->pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  config->alternate = GPIO_alternate_0;

  gpio__init(config, port, pin);
}

void gpio__init_as_input(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin) {
  config->mode = GPIO_mode_INPUT;
  config->type = GPIO_type_PUSH_PULL;
  config->pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  config->alternate = GPIO_alternate_0;

  gpio__init(config, port, pin);
}

// Static functions
// TODO, Analog Mode
static void gpio__update_mode(GPIO_s *config) {
  switch (config->mode) {
  case GPIO_mode_OUTPUT:
    config->port->DIR |= (1 << config->pin);
    break;
  case GPIO_mode_INPUT:
  default:
    config->port->DIR &= ~(1 << config->pin);
    break;
  }
}

static void gpio__update_type(GPIO_s *config) {
  volatile uint32_t *iocon_pin = gpio__get_iocon(config);
  switch (config->type) {
  case GPIO_type_OPEN_DRAIN:
    *iocon_pin |= (1 << IOCON_OD);
    break;
  case GPIO_type_PUSH_PULL:
  default:
    *iocon_pin &= ~(1 << IOCON_OD);
    break;
  }
}

// ? Repeater Mode! Page 132 Table 83
static void gpio__update_pull(GPIO_s *config) {
  volatile uint32_t *iocon_pin = gpio__get_iocon(config);
  uint32_t iocon_data = *iocon_pin;
  iocon_data &= ~(3 << IOCON_MODE);

  switch (config->pull) {
  case GPIO_pull_PULLDOWN:
    iocon_data |= (1 << IOCON_MODE);
    break;
  case GPIO_pull_PULLUP:
    iocon_data |= (1 << IOCON_MODE);
    break;
  case GPIO_pull_NO_PULLUP_OR_PULLDOWN:
  default:
    break;
  }

  *iocon_pin = iocon_data;
}

// NOTE, alternate 8:15 does not exist on this microcontroller
static void gpio__update_alternate_function(GPIO_s *config) {
  volatile uint32_t *iocon_pin = gpio__get_iocon(config);

  GPIO_alternate_e alternate_function = GPIO_alternate_0;
  switch (config->alternate) {
  case GPIO_alternate_0:
  case GPIO_alternate_8:
  case GPIO_alternate_9:
  case GPIO_alternate_10:
  case GPIO_alternate_11:
  case GPIO_alternate_12:
  case GPIO_alternate_13:
  case GPIO_alternate_14:
  case GPIO_alternate_15:
    break;
  default:
    alternate_function = config->alternate;
    break;
  }

  uint32_t iocon_data = *iocon_pin;
  iocon_data &= ~(7 << IOCON_FUNC);
  iocon_data |= ((uint8_t)alternate_function << IOCON_FUNC);
  *iocon_pin = iocon_data;
}

// HELPER STATICS
static volatile uint32_t *gpio__get_iocon(GPIO_s *config) {
  const size_t gpio_port_address = (size_t)config->port;
  // 32 registers each of 4 byte
  // The difference calculated tells us exactly how far the next PORT is from
  // GPIO0
  // i.e GPIO1 is `32 uint32_t` away, GPIO2 is `64 uint32_t` away
  const size_t difference = (gpio_port_address - LPC_GPIO0_BASE) >> 2;

  volatile uint32_t *iocon_port = (volatile uint32_t *)LPC_IOCON;
  iocon_port = iocon_port + difference;
  volatile uint32_t *iocon_pin = (iocon_port + config->pin);
  return iocon_pin;
}
