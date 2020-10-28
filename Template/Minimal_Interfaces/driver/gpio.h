#ifndef INTERFACE_GPIO_H_
#define INTERFACE_GPIO_H_

#include <stdint.h>

// platform specific
#include "gpio/internal.h"

typedef enum {
  GPIO_mode_INPUT,
  GPIO_mode_OUTPUT,
  GPIO_mode_ALTERNATE_FUNCTION,
  GPIO_mode_ANALOG_MODE,
} GPIO_driver_mode_e;

typedef enum {
  GPIO_type_PUSH_PULL,
  GPIO_type_OPEN_DRAIN,
} GPIO_driver_type_e;

typedef struct {
  GPIO_driver_mode_e mode;
  GPIO_driver_type_e type;

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

#endif
