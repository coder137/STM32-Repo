- [Template](#template)
- [Reference Material](#reference-material)
  - [B-L475E-IOT01 User Manual](#b-l475e-iot01-user-manual)
  - [STM32L4x5 Reference Manual](#stm32l4x5-reference-manual)
  - [STM32L475xx Hardware Manual](#stm32l475xx-hardware-manual)
  - [PM0214 Programming Manual](#pm0214-programming-manual)
- [Project Order](#project-order)
- [Other Templates](#other-templates)
- [Document](#document)
  - [Minimal Initial](#minimal-initial)
  - [Minimal Interrupt](#minimal-interrupt)
  - [Minimal FreeRTOS](#minimal-freertos)
  - [Minimal C Stdlib](#minimal-c-stdlib)
  - [Minimal Unit Testing](#minimal-unit-testing)
  - [Minimal Unit Testing Meson](#minimal-unit-testing-meson)

# Template

Application templates have been built on the **B-L475E-IOT01A STM32L4 based board**

- Templates are extensions on Baremetal code
- Templates differ from Baremetal code in the sense that they are on the application level
  - Baremetal code deals with linker script and architecture based initialization
  - Templates are based on microcontroller based code i.e application level

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
- [x] Minimal_FreeRTOS
  - Integrate Amazon FreeRTOS with the project
  - Create example for `task` and `queue` systems
  - Updated interrupt drivers with FreeRTOS functions
  - Fork from `Minimal_Interrupt`
- [x] Minimal_C_Stdlib
  - Contains C Standard Library syscalls
  - Attached `printf` and `scanf` to UART low level driver 
  - Adding openocd debugging
    - openocd commandline
    - openocd cmake integration
    - openocd vscode integration with plugin
  - Fork from `Minimal_FreeRTOS`
- [x] Minimal_Unit_Testing
  - Unity and FFF for Assertion and Mocking
  - CMake and CTest
    - Command line usage
    - VSCode usage
  - Fork from `Minimal_C_Stdlib`

# Other Templates

- [ ] Minimal_ThreadX
  - Integrate Microsoft ThreadX with the project
  - Create example for `task` and `queue` systems
  - Fork from `Minimal_Interrupt` 
- [x] Minimal_Unit_Testing_Meson
  - Unity and FFF for Assertion and Mocking
  - Meson build system
  - Changes to source files have also been added to CMake and tested
  - Forked from `Minimal_Unit_Testing`

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

- [x] Integrate_FreeRTOS
- [x] **FreeRTOS_and_ARM_NVIC**

## Minimal C Stdlib

- [x] Newlib_nano
- [x] Newlib_sbrk
- [x] Newlib_syscalls

- [x] Openocd_gdb_cli
- [x] Openocd_gdb_vscode
- [x] Semihosting

## Minimal Unit Testing

**Open the `Minimal_Unit_Testing` project README**

## Minimal Unit Testing Meson

**Open the `Minimal_Unit_Testing` project README**
