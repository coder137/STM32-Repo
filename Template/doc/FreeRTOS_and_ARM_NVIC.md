- [FreeRTOS and ARM NVIC](#freertos-and-arm-nvic)
- [NVIC Priority Grouping](#nvic-priority-grouping)
- [NVIC Set Priority](#nvic-set-priority)

# FreeRTOS and ARM NVIC

**VERY IMPORTANT**

NVIC Configurations to use FreeRTOS APIs from within an ISR

- [FreeRTOS ARM Cortex M4 Reference](https://www.freertos.org/RTOS-Cortex-M3-M4.html)


# NVIC Priority Grouping

**Preempt Priority and Subpriority**

- It is recommended to assign all the priority bits to be preempt priority bits, leaving no priority bits as subpriority bits. 
  - `NVIC_SetPriorityGrouping` set to `0x03`
  - Read the PM0214 reference manual for the same
- Any other configuration complicates the otherwise direct relationship between the `configMAX_SYSCALL_INTERRUPT_PRIORITY` setting and the priority assigned to individual peripheral interrupts.
- Most systems default to the wanted configuration, with the noticeable exception of the STM32 driver library.
- **If you are using an STM32 with the STM32 driver library then ensure all the priority bits are assigned to be preempt priority bits by calling `NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );` before the RTOS is started.**

# NVIC Set Priority

**Inverse Relationship Between Numeric Priority Value and the Logical Priority Setting**

- FreeRTOS functions that end in `FromISR` are interrupt safe, but even these functions cannot be called from interrupts that have a logical priority above the priority defined by `configMAX_SYSCALL_INTERRUPT_PRIORITY` (`configMAX_SYSCALL_INTERRUPT_PRIORITY` is defined in the FreeRTOSConfig.h header file). 
  - Set the USART1 Interrupt priority to `configMAX_SYSCALL_INTERRUPT_PRIORITY + 1`
  - This makes the USART1 Interrupt to be a lower priority than the FreeRTOS Kernel
- Therefore, any interrupt service routine that uses an RTOS API function must have its priority manually set to a value that is numerically equal to or greater than the `configMAX_SYSCALL_INTERRUPT_PRIORITY` setting. 
- This ensures the interrupt’s logical priority is equal to or less than the `configMAX_SYSCALL_INTERRUPT_PRIORITY` setting.
- **Cortex-M interrupts default to having a priority value of zero. Zero is the highest possible priority value. Therefore, never leave the priority of an interrupt that uses the interrupt safe RTOS API at its default value.**
