#include "system_lpc4078xx.h"
#include "lpc4078xx.h"

static void system__initialize_fpu(void);
static void system__initialize_clock(void);

void SystemInit() {
  system__initialize_fpu();
  system__initialize_clock();
}

// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0439b/BEHBJHIG.html
static void system__initialize_fpu(void) { SCB->CPACR |= (0xF << 20); }

// 3.10.6 PLL Configuration sequence (User Manual)
static void system__initialize_clock(void) {
  const uint8_t multiplier = 8;
  LPC_SC->PLL0CFG |= (multiplier - 1) << 0;
  LPC_SC->PLL0CON = 1;

  // PLLCON PLLE
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;

  // 4 Clock dividers
  LPC_SC->PCLKSEL = 1; // APB peripherals receive clock/1 (divided by 1)

  // 5: Wait for PLL to lock
  const uint32_t pll_lock_bit_mask = (1 << 10);
  while (!(LPC_SC->PLL0STAT & pll_lock_bit_mask)) {
    ;
  }

  // Before we switch to the faster clock, we need to configure flash memory
  // access properly with respect to core clock
  uint32_t flash_cfg = LPC_SC->FLASHCFG;
  flash_cfg &= ~(0xF << 12);
  flash_cfg |= (0x4 << 12); // 5 cpu clocks for use with up to 100Mhz
  LPC_SC->FLASHCFG = flash_cfg;

  // Step 6 from UM: Connect the PLL to the CPU
  const uint32_t cpu_on_pll_clock = (1 << 8) | (1 << 0);
  LPC_SC->CCLKSEL = cpu_on_pll_clock; // CPU uses PLL clock (Table 20)
}
