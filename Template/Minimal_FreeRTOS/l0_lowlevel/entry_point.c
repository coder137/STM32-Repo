#include "FreeRTOSConfig.h"
#include "stm32l4xx.h"

extern int main();

void _start(void) {
  // Read the SCB->AIRCR Register in the PM0214 Programming Manual
  NVIC_SetPriorityGrouping(0x03);

  // TODO, Initialize the C Standard Library here if required

  main();

  // Should never return
  while(1) {}
}
