#include "fff.h"
#include "unity.h"

#include "output_gpio.h"

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(rcc__set_ahb2_peripheral_clock_enable, uint32_t);
FAKE_VOID_FUNC(gpio__init, GPIO_s *, GPIO_TypeDef *, uint32_t);

void setUp() {}
void tearDown() {}

void gpio_output_init_test(void) {
  GPIO_s output_config;
  gpio_output__init(&output_config, GPIOA, 5, RCC_AHB2ENR_GPIOAEN);

  TEST_ASSERT_EQUAL_UINT16(
      rcc__set_ahb2_peripheral_clock_enable_fake.call_count, 1);
  TEST_ASSERT_EQUAL_UINT16(gpio__init_fake.call_count, 1);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(gpio_output_init_test);
  return UNITY_END();
}
