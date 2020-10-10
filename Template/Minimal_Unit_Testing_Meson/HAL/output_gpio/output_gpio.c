#include "output_gpio.h"

#include "rcc/rcc.h"

void output_gpio__init(GPIO_s *output_config, GPIO_TypeDef *port, uint32_t pin,
                       uint32_t rcc_config) {
  rcc__set_ahb2_peripheral_clock_enable(rcc_config);

  output_config->mode = GPIO_mode_OUTPUT;
  output_config->type = GPIO_type_PUSH_PULL;
  output_config->speed = GPIO_speed_LOW_SPEED;
  output_config->pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  gpio__init(output_config, port, pin);
}
