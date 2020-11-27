#include "exti.h"

#include "stm32l475xx.h"

// STATIC FUNCTION DECLARATION
static void exti__update_imr1(EXTI_s *config);
static void exti__update_emr1(EXTI_s *config);
static void exti__update_type(EXTI_s *config);

void exti__gpio_register_interrupt(EXTI_s *config) {
  exti__update_imr1(config);
  exti__update_emr1(config);
  exti__update_type(config);
}

bool exti__gpio_is_pending_interrupt(EXTI_s *config) {
  bool ispending = (EXTI->PR1 >> config->pin) & 0x01;
  return ispending;
}

void exti__gpio_clear_pending_interrupt(EXTI_s *config) {
  EXTI->PR1 |= (1 << config->pin);
}

// STATIC FUNCTION DEFINITION

static void exti__update_imr1(EXTI_s *config) {
  uint32_t imr1_data = EXTI->IMR1;

  imr1_data &= ~(1 << config->pin);
  imr1_data |= (1 << config->pin);

  EXTI->IMR1 = imr1_data;
}

// TODO,
static void exti__update_emr1(EXTI_s *config) {
  uint32_t emr1_data = EXTI->EMR1;

  emr1_data &= ~(1 << config->pin);
  emr1_data |= (1 << config->pin);

  EXTI->EMR1 = emr1_data;
}

static void exti__update_type(EXTI_s *config) {
  uint32_t rtsr1_data = EXTI->RTSR1;
  uint32_t ftsr1_data = EXTI->FTSR1;

  ftsr1_data &= ~(1 << config->pin);
  rtsr1_data &= ~(1 << config->pin);

  switch (config->type) {
  case EXTI_type_RISING:
    rtsr1_data |= (1 << config->pin);
    break;
  case EXTI_type_FALLING:
    ftsr1_data |= (1 << config->pin);
    break;
  case EXTI_type_RISING_FALLING:
    rtsr1_data |= (1 << config->pin);
    ftsr1_data |= (1 << config->pin);
    break;
  default:
    break;
  }

  EXTI->FTSR1 = ftsr1_data;
  EXTI->RTSR1 = rtsr1_data;
}
