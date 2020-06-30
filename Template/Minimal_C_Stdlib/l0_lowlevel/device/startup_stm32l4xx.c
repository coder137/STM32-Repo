/******************************************************************************
 * @file     startup_ARMCM4.c
 * @brief    CMSIS-Core(M) Device Startup File for a Cortex-M4 Device
 * @version  V2.0.3
 * @date     31. March 2020
 ******************************************************************************/
/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * USER ADDDED
 */
#include "FreeRTOS.h"
#include "stm32l4xx.h"

typedef void (*VECTOR_TABLE_Type)(void);

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;

extern __NO_RETURN void __PROGRAM_START(void);

extern void vPortSVCHandler(void);
extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void);
void Default_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

void WWDG_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PVD_PVM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_TAMP_STAMP_Handler(void)
    __attribute__((weak, alias("Default_Handler")));
void RTC_WKUP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FLASH_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RCC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC1_2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_TX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_RX0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_ALARM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DFSDM1_FLT3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_BRK_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_UP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_TRG_COM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_CC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FMC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SDMMC1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM6_DAC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DFSDM1_FLT0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DFSDM1_FLT1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DFSDM1_FLT2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void COMP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void LPTIM1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void LPTIM2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void OTG_FS_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void LPUART1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void QUADSPI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_EV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_ER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SAI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SAI2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SWPMI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TSC_Handler(void) __attribute__((weak, alias("Default_Handler")));

void RNG_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FPU_Handler(void) __attribute__((weak, alias("Default_Handler")));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
    (VECTOR_TABLE_Type)(&__INITIAL_SP), /*     Initial Stack Pointer */
    Reset_Handler,                      /*     Reset Handler */
    NMI_Handler,                        /* -14 NMI Handler */
    HardFault_Handler,                  /* -13 Hard Fault Handler */
    MemManage_Handler,                  /* -12 MPU Fault Handler */
    BusFault_Handler,                   /* -11 Bus Fault Handler */
    UsageFault_Handler,                 /* -10 Usage Fault Handler */
    0,                                  /*     Reserved */
    0,                                  /*     Reserved */
    0,                                  /*     Reserved */
    0,                                  /*     Reserved */
    vPortSVCHandler,                    /*  -5 SVCall Handler */
    DebugMon_Handler,                   /*  -4 Debug Monitor Handler */
    0,                                  /*     Reserved */
    xPortPendSVHandler,                 /*  -2 PendSV Handler */
    xPortSysTickHandler,                /*  -1 SysTick Handler */

    /* Interrupts */
    WWDG_Handler,
    PVD_PVM_Handler,
    RTC_TAMP_STAMP_Handler,
    RTC_WKUP_Handler,
    FLASH_Handler,
    RCC_Handler,
    EXTI0_Handler,
    EXTI1_Handler,
    EXTI2_Handler,
    EXTI3_Handler,
    EXTI4_Handler,
    DMA1_CH1_Handler,
    DMA1_CH2_Handler,
    DMA1_CH3_Handler,
    DMA1_CH4_Handler,
    DMA1_CH5_Handler,
    DMA1_CH6_Handler,
    DMA1_CH7_Handler,
    ADC1_2_Handler,
    CAN1_TX_Handler,
    CAN1_RX0_Handler,
    CAN1_RX1_Handler,
    CAN1_SCE_Handler,
    EXTI9_5_Handler,
    TIM1_BRK_Handler,
    TIM1_UP_Handler,
    TIM1_TRG_COM_Handler,
    TIM1_CC_Handler,
    TIM2_Handler,
    TIM3_Handler,
    TIM4_Handler,
    I2C1_EV_Handler,
    I2C1_ER_Handler,
    I2C2_EV_Handler,
    I2C2_ER_Handler,
    SPI1_Handler,
    SPI2_Handler,
    USART1_Handler,
    USART2_Handler,
    USART3_Handler,
    EXTI15_10_Handler,
    RTC_ALARM_Handler,
    DFSDM1_FLT3_Handler,
    TIM8_BRK_Handler,
    TIM8_UP_Handler,
    TIM8_TRG_COM_Handler,
    TIM8_CC_Handler,
    ADC3_Handler,
    FMC_Handler,
    SDMMC1_Handler,
    TIM5_Handler,
    SPI3_Handler,
    UART4_Handler,
    UART5_Handler,
    TIM6_DAC_Handler,
    TIM7_Handler,
    DMA2_CH1_Handler,
    DMA2_CH2_Handler,
    DMA2_CH3_Handler,
    DMA2_CH4_Handler,
    DMA2_CH5_Handler,
    DFSDM1_FLT0_Handler,
    DFSDM1_FLT1_Handler,
    DFSDM1_FLT2_Handler,
    COMP_Handler,
    LPTIM1_Handler,
    LPTIM2_Handler,
    OTG_FS_Handler,
    DMA2_CH6_Handler,
    DMA2_CH7_Handler,
    LPUART1_Handler,
    QUADSPI_Handler,
    I2C3_EV_Handler,
    I2C3_ER_Handler,
    SAI1_Handler,
    SAI2_Handler,
    SWPMI1_Handler,
    TSC_Handler,
    0,
    0,
    RNG_Handler,
    FPU_Handler,
};

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void) {
  SystemInit();      /* CMSIS System Initialization */
  __PROGRAM_START(); /* Enter PreMain (C library entry point) */
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

/*----------------------------------------------------------------------------
  Hard Fault Handler
 *----------------------------------------------------------------------------*/
void HardFault_Handler(void) {
  while (1)
    ;
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void) {
  while (1)
    ;
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic pop
#endif
