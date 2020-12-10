#include "rcc.h"
#include "stm32l475xx.h"

void rcc__set_ahb2_peripheral_clock_enable(uint32_t ahb2) {
  RCC->AHB2ENR |= (ahb2);
}
