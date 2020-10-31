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

typedef enum {
  GPIO_alternate_0,
  GPIO_alternate_1,
  GPIO_alternate_2,
  GPIO_alternate_3,
  GPIO_alternate_4,
  GPIO_alternate_5,
  GPIO_alternate_6,
  GPIO_alternate_7,

  GPIO_alternate_8,
  GPIO_alternate_9,
  GPIO_alternate_10,
  GPIO_alternate_11,
  GPIO_alternate_12,
  GPIO_alternate_13,
  GPIO_alternate_14,
  GPIO_alternate_15,
} GPIO_alternate_e;

typedef GPIO_TypeDef GPIO_TypeDef;

// Internal data requirements
typedef struct {
  GPIO_speed_e speed;
  GPIO_alternate_e alternate;
} GPIO_internal_s;

// NOTE, Should not contains any architecture specific functions

#endif
