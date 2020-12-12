#ifndef MODULES_EXTI_H
#define MODULES_EXTI_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  EXTI_type_RISING,
  EXTI_type_FALLING,
  EXTI_type_RISING_FALLING,
} EXTI_type_e;

typedef struct {
  EXTI_type_e type;
  uint8_t pin;

} EXTI_s;

void exti__gpio_register_interrupt(EXTI_s *config);
bool exti__gpio_is_pending_interrupt(EXTI_s *config);
void exti__gpio_clear_pending_interrupt(EXTI_s *config);

/**
 * TODO,
 * Event Registration (EMR)
 * Software Interrupt (SWIER)
 */

#endif
