#ifndef HAL_INPUT_GPIO_INPUT_GPIO_H_
#define HAL_INPUT_GPIO_INPUT_GPIO_H_

#include "gpio.h"

void input_gpio__init(GPIO_s *input_config, GPIO_TypeDef *port, uint32_t pin,
                      uint32_t rcc_config);

#endif
