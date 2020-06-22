- [Template](#template)
- [Reference Material](#reference-material)
  - [B-L475E-IOT01 User Manual](#b-l475e-iot01-user-manual)
  - [STM32L4x5 Reference Manual](#stm32l4x5-reference-manual)
  - [STM32L475xx Hardware Manual](#stm32l475xx-hardware-manual)
  - [PM0214 Programming Manual](#pm0214-programming-manual)
- [Project Order](#project-order)
- [Document](#document)
  - [Minimal Initial](#minimal-initial)
  - [Minimal Interrupt](#minimal-interrupt)
  - [Minimal FreeRTOS](#minimal-freertos)
  - [Minimal C Stdlib](#minimal-c-stdlib)

# Template

Templates for starting STM32 Projects

# Reference Material 

> See `STM32-Repo/doc` folder

## B-L475E-IOT01 User Manual

**Dev Board Manual**

- Contains the **Board Schematic** and **Pin Configurations** for GPIO and UART.
- Contains the external **Hardware Modules** connected to the Development Board

## STM32L4x5 Reference Manual

**Generic Chip Level Manual**

- Contains the **Technical Reference Material** for all the STM32L4x5 Microcontroller family

## STM32L475xx Hardware Manual

**Chip Specific Manual**

- The B-L475E-IOT01 Uses the **STM32l475xx** chip
- This contains relevant information pertaining to each chip within the generic family

## PM0214 Programming Manual

**Generic ARM level Manual**

- STM32 is based on the ARM Architecture and follows the ARM CMSIS standard
- This manual shows the various ARM Register level configurations that are common to **all** ARM STM32 devices.

# Project Order

- [x] Minimal_Initial
  - Writing 2 low level drivers GPIO and UART
- [x] Minimal_Interrupt
  - Writing GPIO and UART using interrupts
- [ ] Minimal_FreeRTOS
  - Integrate Amazon FreeRTOS with the project
  - Create example for `task` and `queue` systems
- [ ] Minimal_C_Stdlib
  - Contains C Standard Library syscalls
  - Updated interrupt drivers with FreeRTOS functions
  - Attached `printf` and `scanf` to UART low level driver 

# Document

> See the `STM32-Repo/Template/doc` folder

- General Method of how the drivers were written

## Minimal Initial

- [x] Base_GPIO
- [x] Base_UART

## Minimal Interrupt

- [x] Vector_Interrupt
- [x] EXTI_GPIO
- [x] Interrupt_UART

## Minimal FreeRTOS

- [ ] Integrate_FreeRTOS

## Minimal C Stdlib

- [ ] Integrate_Newlib
