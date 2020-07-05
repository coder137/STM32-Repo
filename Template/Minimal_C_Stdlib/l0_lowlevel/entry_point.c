#include "FreeRTOSConfig.h"
#include "stm32l4xx.h"

extern void peripheral__initialize(void);
extern int main();

void _start(void) {
  // Read the SCB->AIRCR Register in the PM0214 Programming Manual
  NVIC_SetPriorityGrouping(0x03);

  // l4_user_module
  peripheral__initialize();

  // l5_application
  main();

  // Should never return
  while (1) {
  }
}
