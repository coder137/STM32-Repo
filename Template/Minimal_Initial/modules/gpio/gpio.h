#ifndef MODULES_GPIO_H
#define MODULES_GPIO_H

#include <stdbool.h>
#include <stdint.h>

#include "stm32l4xx.h"

typedef enum {
  GPIO_mode_INPUT,
  GPIO_mode_OUTPUT,
  GPIO_mode_ALTERNATE_FUNCTION,
  GPIO_mode_ANALOG_MODE,
} GPIO_mode_e;

typedef enum {
  GPIO_type_PUSH_PULL,
  GPIO_type_OPEN_DRAIN,
} GPIO_type_e;

typedef enum {
  GPIO_speed_LOW_SPEED,
  GPIO_speed_MEDIUM_SPEED,
  GPIO_speed_HIGH_SPEED,
  GPIO_speed_VERY_HIGH_SPEED,
} GPIO_speed_e;

typedef enum {
  GPIO_pull_NO_PULLUP_OR_PULLDOWN,
  GPIO_pull_PULLUP,
  GPIO_pull_PULLDOWN,
  GPIO_pull_RESERVED,
} GPIO_pull_e;

// Main GPIO Configuration
typedef struct {
  GPIO_TypeDef *port;
  uint32_t pin;

  GPIO_mode_e mode;
  GPIO_type_e type;
  GPIO_speed_e speed;
  GPIO_pull_e pull;
} GPIO_s;

void gpio__init(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin);

void gpio__set(const GPIO_s *config);
void gpio__reset(const GPIO_s *config);

#endif
