#ifndef HAL_OUTPUT_GPIO_OUTPUT_GPIO_H_
#define HAL_OUTPUT_GPIO_OUTPUT_GPIO_H_

#include "gpio/gpio.h"

void gpio_output__init(GPIO_s *output_config, GPIO_TypeDef *port, uint32_t pin,
                       uint32_t rcc_config);

#endif
