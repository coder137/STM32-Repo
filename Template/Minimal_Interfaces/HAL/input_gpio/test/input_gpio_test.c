#include "fff.h"
#include "unity.h"

#include "mock_gpio.h"
#include "mock_rcc.h"

#include "input_gpio.h"

DEFINE_FFF_GLOBALS;

void setUp() {
  RESET_FAKE(rcc__set_ahb2_peripheral_clock_enable);
  RESET_FAKE(gpio__init);

  FFF_RESET_HISTORY();
}

void tearDown() {}

void gpio_input_init_test(void) {
  GPIO_s input_config;
  input_gpio__init(&input_config, GPIOC, 13, RCC_AHB2ENR_GPIOCEN);

  TEST_ASSERT_EQUAL_UINT16(
      rcc__set_ahb2_peripheral_clock_enable_fake.call_count, 1);
  TEST_ASSERT_EQUAL_UINT16(gpio__init_fake.call_count, 1);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(gpio_input_init_test);
  return UNITY_END();
}
