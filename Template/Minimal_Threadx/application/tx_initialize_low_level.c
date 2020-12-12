/**
 * Comparison between .asm and .c version of `tx_initialize_low_level`
 * .asm file -> Os compilation
 * text	   data	    bss	    dec	    hex
 * 7916	    128	  11004	  19048	   4a68
 *
 * .c file -> Os compilation
 * text	   data	    bss	    dec	    hex
 * 7884	    128	  11004	  19016	   4a48
 *
 * .asm file -> O0 compilation
 * text	   data	    bss	    dec	    hex
 * 12243	 129	  11004	  23376	   5b50
 *
 * .c file -> O0 compilation
 * text	   data	    bss	    dec	    hex
 * 12307	 129	  11004	  23440	   5b90
 *
 * Size comparison
 * Os compilation -> .c binary is 32 bytes smaller
 * O0 compilation -> .c binary is 64 bytes larger
 *
 */
#include "stm32l4xx.h"
#include <ctype.h>

// Threadx variables
extern void *_tx_initialize_unused_memory;
extern void *_tx_thread_system_stack_ptr;

// Linker script symbols
extern void *__HeapLimit;
// CMSIS Wrapper points to `__StackTop` in the linker script
extern uint32_t __INITIAL_SP;

void _tx_initialize_low_level(void) {
  // Disable all the interrupts
  __disable_irq();

  // Set base of available memory to end of non-initialised RAM area
  size_t ram_segment_used_end_address = (size_t)&__HeapLimit;
  ram_segment_used_end_address += 4;
  _tx_initialize_unused_memory = (void *)ram_segment_used_end_address;

  // Setup Vector Table Offset
  // This is where the Vector table resides
  // * NOTE, We can also omit this since it has been added to `SystemInit`
  SCB->VTOR = FLASH_BASE;

  // Set system stack pointer from vector value
  // __INITIAL_SP is defined above (CMSIS usage)
  // We can also get this value from the linker script __StackTop
  _tx_thread_system_stack_ptr = (void *)&__INITIAL_SP;

  // Enable the cycle count register
  // 0 -> CYCCNTENA
  // ARMv7-M Architecture Reference Manual: Page C1-737
  DWT->CTRL |= (1 << 0);

  // Configure SysTick for 100Hz clock, or 16384 cycles if no reference
  const size_t SYSTICK_CYCLES = ((SystemCoreClock / 100) - 1);
  SysTick->LOAD = SYSTICK_CYCLES;
  SysTick->CTRL = 0x7;

  // Configure handler priorities
  // TODO, Find a better way to do this
  SCB->SHP[0] = 0;
  SCB->SHP[1] = 0;
  SCB->SHP[2] = 0;
  // SCB->SHP[3] = 0; // reserved

  // SCB->SHP[4] = 0; // reserved
  // SCB->SHP[5] = 0; // reserved
  // SCB->SHP[6] = 0; // reserved
  SCB->SHP[7] = 0xFF;

  // SCB->SHP[8] = 0; // reserved
  // SCB->SHP[9] = 0; // reserved
  SCB->SHP[10] = 0xFF;
  SCB->SHP[11] = 0x40;
}

// Handlers

// Generic Interrupt handler template

void __tx_IntHandler(void) {
#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
  _tx_execution_isr_enter();
#endif
#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
  _tx_execution_isr_exit();
#endif
}

// System Tick timer interrupt handler

extern void _tx_timer_interrupt();
static inline void SystickHandlerHelper(void) {
#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
  _tx_execution_isr_enter();
#endif

  // See tx_timer_interrupt.S in threadx/ports/cortex_m4/gnu/src
  _tx_timer_interrupt();

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
  _tx_execution_isr_exit();
#endif
}

void __tx_SysTickHandler(void) { SystickHandlerHelper(); }
void SysTick_Handler(void) { SystickHandlerHelper(); }

// * Are the below functions used anywhere?

void __tx_BadHandler(void) {
  while (1) {
  }
}

void __tx_HardfaultHandler(void) {
  while (1) {
  }
}

void __tx_SVCallHandler(void) {
  while (1) {
  }
}

void __tx_NMIHandler(void) {
  while (1) {
  }
}

void __tx_DBGHandler(void) {
  while (1) {
  }
}
