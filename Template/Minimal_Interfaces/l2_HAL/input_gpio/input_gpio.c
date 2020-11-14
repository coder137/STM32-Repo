#include "input_gpio.h"

#include "rcc/rcc.h"

void input_gpio__init(GPIO_s *input_config, GPIO_TypeDef *port, uint32_t pin,
                      uint32_t rcc_config) {
  rcc__set_ahb2_peripheral_clock_enable(rcc_config);

  input_config->mode = GPIO_mode_INPUT;
  input_config->type = GPIO_type_PUSH_PULL;
  input_config->_internal.speed = GPIO_speed_LOW_SPEED;
  input_config->pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  gpio__init(input_config, port, pin);
}
