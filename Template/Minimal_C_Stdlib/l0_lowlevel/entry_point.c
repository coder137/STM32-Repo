#include "FreeRTOSConfig.h"
#include "stm32l4xx.h"

extern void peripheral__initialize(void);
extern int main();

extern void initialise_monitor_handles(void);


void _start(void) {
  // Read the SCB->AIRCR Register in the PM0214 Programming Manual
  NVIC_SetPriorityGrouping(0x03);

  // Semihosting
#if SEMIHOSTING
  initialise_monitor_handles();
#endif

  // l4_user_module
  peripheral__initialize();

  // l5_application
  main();

  // Should never return
  while (1) {
  }
}
