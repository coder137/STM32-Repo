#include "FreeRTOSConfig.h"
#include "stm32l475xx.h"

// TODO, Shift this to a different file
static void clock__init(void);
static void clock__update_flash_latency(void);
static void clock__configure_msi_clock(void);
static void clock__configure_pll_clock(void);
static void clock__configure_pll_as_sysclk(void);

extern void peripheral__initialize(void);
extern int main();

extern void initialise_monitor_handles(void);

void _start(void) {
  // l0_lowlevel
  clock__update_flash_latency();
  clock__init();

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

// CONSTANTS
static const uint8_t BITMASK_1 = 1;
static const uint8_t BITMASK_2 = 3;
static const uint8_t BITMASK_3 = 7;

// FLASH
static const uint8_t ACR_LATENCY = 0;

// RCC
static const uint8_t CR_MSION = 0;
static const uint8_t CR_MSIRDY = 1;
static const uint8_t CR_PLLON = 24;
static const uint8_t CR_PLLRDY = 25;

static const uint8_t CFGR_SW = 0;

static const uint8_t PLLCFGR_PLLSRC = 0;
static const uint8_t PLLCFGR_PLLM = 4;
static const uint8_t PLLCFGR_PLLN = 8;

static const uint8_t PLLCFGR_PLLP = 17;
static const uint8_t PLLCFGR_PLLQ = 21;
static const uint8_t PLLCFGR_PLLREN = 24;
static const uint8_t PLLCFGR_PLLR = 25;

// STM32L4x5 Reference Manual: Page 214
static void clock__init() {
  clock__configure_msi_clock();
  clock__configure_pll_clock();
  clock__configure_pll_as_sysclk();
  SystemCoreClockUpdate();
}

// 3.3.3 FLASH (Read Access Latency) Page 100 RM0351
static void clock__update_flash_latency(void) {
  const uint8_t WAIT_STATES = 4;

  uint32_t flash_acr = FLASH->ACR;
  flash_acr &= ~(BITMASK_3 << ACR_LATENCY);
  flash_acr |= (WAIT_STATES << ACR_LATENCY);
  FLASH->ACR = flash_acr;
}

static void clock__configure_msi_clock(void) {
  // Make sure MSI is ON
  // Set the MSI PLL ON
  uint32_t cr = RCC->CR;
  cr |= (1 << CR_MSION);
  RCC->CR = cr;

  // Wait while it is not ready
  while ((RCC->CR & (1 << CR_MSIRDY)) == 0) {
  }
}

static void clock__configure_pll_clock(void) {
  // Turn off PLL to make changes
  // Make sure the PLL is unlocked (sets to 0)
  RCC->CR &= ~(BITMASK_1 << CR_PLLON);
  while ((RCC->CR & (1 << CR_PLLRDY)) == 1) {
  }

  // PLLM
  // PLLN
  // Reset PLLM and PLLN
  const uint8_t BITMASK_7 = 0x7F;
  const uint8_t PLLN_MULTIPLIER = 40;
  uint32_t pllcfgr = RCC->PLLCFGR;
  pllcfgr &= ~((BITMASK_3 << PLLCFGR_PLLM) | (BITMASK_7 << PLLCFGR_PLLN) |
               (BITMASK_2 << PLLCFGR_PLLSRC));
  pllcfgr |= ((PLLN_MULTIPLIER << PLLCFGR_PLLN) | (1 << PLLCFGR_PLLSRC));

  // PLLP, PLLQ, PLLREN
  // Only PLLR is needed here
  pllcfgr &= ~((BITMASK_1 << PLLCFGR_PLLP) | (BITMASK_2 << PLLCFGR_PLLQ) |
               (BITMASK_2 << PLLCFGR_PLLR));
  pllcfgr |= ((1 << PLLCFGR_PLLREN));
  RCC->PLLCFGR = pllcfgr;

  // Turn on PLL
  // Wait until the PLL is locked
  RCC->CR |= (1 << CR_PLLON);
  while ((RCC->CR & (1 << CR_PLLRDY)) == 0) {
  }
}

static void clock__configure_pll_as_sysclk(void) {
  const uint8_t PLL_SYSCLK = 3;
  uint32_t cfgr = RCC->CFGR;
  cfgr &= ~(BITMASK_2 << CFGR_SW);
  cfgr |= (PLL_SYSCLK << CFGR_SW);
  RCC->CFGR = cfgr;
}
