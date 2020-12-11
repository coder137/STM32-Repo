#ifndef DRIVER_RCC_RCC_H_
#define DRIVER_RCC_RCC_H_

#include <stdint.h>

/**
 * @brief RCC Register Description
 *
 * CR ( Clock Control Register )
 * ICSCR ( Internal Clock Sources Calibration Register )
 * CFGR ( Clock Configuration Register )
 * PLLCFGR ( PLL Configuration Register )
 * PLLSAI1CFGR ( PLLSAI1 Configuration Register )
 * PLLSAI2CFGR ( PLLSAI2 Configuration Register )
 * CIER ( Clock Interrupt Enable Register )
 * CIFR ( Clock Interrupt Flag Register )
 * CICR ( Clock Interrupt Clear Register )
 * AHB1RSTR ( AHB1 Peripheral Reset Register )
 * AHB2RSTR ( AHB2 Peripheral Reset Register )
 * AHB3RSTR ( AHB3 Peripheral Reset Register )
 * APB1RSTR1 ( APB1 Peripheral Reset Register 1 )
 * APB1RSTR2 ( APB1 Peripheral Reset Register 2 )
 * APB2RSTR ( APB2 Peripheral Reset Register )
 * AHB1ENR ( AHB1 Peripheral Clock Enable Register )
 * AHB2ENR ( AHB2 Peripheral Clock Enable Register )
 * AHB3ENR ( AHB3 Peripheral Clock Enable Register )
 * APB1ENR1 ( APB1 Peripheral Clock Enable Register 1 )
 * APB1ENR2 ( APB1 Peripheral Clock Enable Register 2 )
 * APB2ENR ( APB2 Peripheral Clock Enable Register )
 * AHB1SMENR ( AHB1 peripheral clocks enable in Sleep and Stop modes register )
 * AHB2SMENR ( AHB2 peripheral clocks enable in Sleep and Stop modes register )
 * AHB3SMENR ( AHB3 peripheral clocks enable in Sleep and Stop modes register )
 * APB1SMENR1 ( APB1 peripheral clocks enable in Sleep and Stop modes register 1
 * ) APB1SMENR2 ( APB1 peripheral clocks enable in Sleep and Stop modes register
 * 2 ) APB2SMENR ( APB2 peripheral clocks enable in Sleep and Stop modes
 * register ) CCIPR ( Peripherals independent clock configuration register )
 * BDCR ( Backup domain control register )
 * CSR ( Control/status register )
 * CRRCR ( Clock recovery RC register )
 * CCIPR2 ( Peripherals independent clock configuration register 2 )
 */

/**
 * @brief Set the ABH2 Peripheral Clock Enable register
 * Can be a combination of
 * ahb2 = (RCC_AESEN | RCC_HASHEN) etc
 * @param ahb2
 */
void rcc__set_ahb2_peripheral_clock_enable(uint32_t ahb2);

#endif
