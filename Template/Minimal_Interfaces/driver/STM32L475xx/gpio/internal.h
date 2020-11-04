#ifndef DRIVER_GPIO_INTERNAL_H_
#define DRIVER_GPIO_INTERNAL_H_

#include <stdbool.h>
#include <stdint.h>

#include "stm32l4xx.h"

typedef enum {
  GPIO_speed_LOW_SPEED,
  GPIO_speed_MEDIUM_SPEED,
  GPIO_speed_HIGH_SPEED,
  GPIO_speed_VERY_HIGH_SPEED,
} GPIO_speed_e;

typedef GPIO_TypeDef GPIO_TypeDef;

// Internal data requirements
typedef struct {
  GPIO_speed_e speed;
} GPIO_internal_s;

// NOTE, Should not contains any architecture specific functions

#endif
