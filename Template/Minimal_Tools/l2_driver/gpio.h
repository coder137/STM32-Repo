#ifndef DRIVER_INTERFACE_GPIO_H_
#define DRIVER_INTERFACE_GPIO_H_

#include <stdbool.h>
#include <stdint.h>

// platform specific
#include "gpio/internal.h"

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
  GPIO_pull_NO_PULLUP_OR_PULLDOWN,
  GPIO_pull_PULLUP,
  GPIO_pull_PULLDOWN,
} GPIO_pull_e;

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

typedef struct {
  GPIO_mode_e mode;
  GPIO_type_e type;
  GPIO_pull_e pull;
  GPIO_alternate_e alternate;

  // Hardware specific should be typedef'ed
  GPIO_TypeDef *port;
  uint32_t pin;

  // platform specific
  GPIO_internal_s _internal;
} GPIO_s;

// Define these in the .c file
void gpio__init(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin);

void gpio__write(const GPIO_s *config, uint8_t value);
void gpio__set(const GPIO_s *config);
void gpio__reset(const GPIO_s *config);

bool gpio__read(const GPIO_s *config);

// High level APIs
void gpio__init_as_output(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin);
void gpio__init_as_input(GPIO_s *config, GPIO_TypeDef *port, uint32_t pin);

#endif
