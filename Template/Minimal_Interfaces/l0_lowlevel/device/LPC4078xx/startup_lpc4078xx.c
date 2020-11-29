#include "lpc4078xx.h"
#include "system_lpc4078xx.h"

typedef void (*VECTOR_TABLE_Type)(void);

// External References
// CMSIS
extern uint32_t __INITIAL_SP;
extern __NO_RETURN void __PROGRAM_START(void);

// FreeRTOS
extern void vPortSVCHandler(void);
extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);

// Internal References
__NO_RETURN void Reset_Handler(void);
void Default_Handler(void);

// Exception / Interrupt Handler
// Exceptions
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

// Interrupt Handler
void WDT_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SSP0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SSP1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PLL0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EINT0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EINT1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EINT2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EINT3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BOD_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USB_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2S_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ENET_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MCI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MCPWM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void QEI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PLL1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USBActivity_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CANActivity_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SSP2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void LCD_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EEPROM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CMP0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CMP1_Handler(void) __attribute__((weak, alias("Default_Handler")));

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

    WDT_Handler,
    TIMER0_Handler,
    TIMER1_Handler,
    TIMER2_Handler,
    TIMER3_Handler,
    UART0_Handler,
    UART1_Handler,
    UART2_Handler,
    UART3_Handler,
    PWM1_Handler,
    I2C0_Handler,
    I2C1_Handler,
    I2C2_Handler,
    0,
    SSP0_Handler,
    SSP1_Handler,
    PLL0_Handler,
    RTC_Handler,
    EINT0_Handler, // 34 External Interrupt
    EINT1_Handler, // 35 External Interrupt
    EINT2_Handler, // 36 External Interrupt
    EINT3_Handler, // 37 External Interrupt
    ADC_Handler,
    BOD_Handler, // 39 Brown Out Detector
    USB_Handler,
    CAN_Handler,
    DMA_Handler,
    I2S_Handler,
    ENET_Handler,  // 44 Ethernet
    MCI_Handler,   // 45 SD Card Interface
    MCPWM_Handler, // 46 Motor Control PWM
    QEI_Handler,   // 47 Quadrature Encoder
    PLL1_Handler,
    USBActivity_Handler,
    CANActivity_Handler,
    UART4_Handler,
    SSP2_Handler,
    LCD_Handler,
    GPIO_Handler,
    PWM0_Handler,
    EEPROM_Handler,
};

// Reset Handler called on controller reset
__NO_RETURN void Reset_Handler(void) {
  SystemInit();      /* CMSIS System Initialization */
  __PROGRAM_START(); /* Enter PreMain (C library entry point) */
}

// Hard Fault Handler
void HardFault_Handler(void) {
  while (1)
    ;
}

// Default Handler for Exceptions / Interrupts
void Default_Handler(void) {
  while (1)
    ;
}
