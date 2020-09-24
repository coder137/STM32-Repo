- [STM32-Repo](#stm32-repo)
  - [Pre-Requisites](#pre-requisites)
  - [Links](#links)
    - [STM32L4](#stm32l4)
    - [STM32F7](#stm32f7)
- [Repository Structure](#repository-structure)
- [Project Structure](#project-structure)
  - [Device Dependent folders](#device-dependent-folders)
    - [l0_device_specific](#l0_device_specific)
    - [l1_third_party_device_dependent](#l1_third_party_device_dependent)
    - [l2_drivers](#l2_drivers)
  - [Device Independent folders](#device-independent-folders)
    - [l3_board_specific](#l3_board_specific)
    - [l3_functional_third_party](#l3_functional_third_party)
    - [l4_user_module](#l4_user_module)
    - [l5_application](#l5_application)
- [Roadmap](#roadmap)

# STM32-Repo

- STM32 Repository for learning HAL and CMSIS (Baremetal)
- Developing an Enterprise-level Embedded software stack from scratch by identifying and integrating required components

## Pre-Requisites

- `arm-none-eabi-*` GNU Toolchain
- CMake
- Ninja
- STM32CubeProgrammer
- OpenOCD
- VSCode Plugins
  - CMake
  - Cortex-Debug
  - C/C++

## Links

### STM32L4

- [STM32 Website for B-L475E-IOT01A](https://www.st.com/en/evaluation-tools/b-l475e-iot01a.html)
- STM32L475_x examples use [the B-L475E-IOT01A board](https://os.mbed.com/platforms/ST-Discovery-L475E-IOT01A/) 

### STM32F7

- [STM32 Website for NUCLEO-F746ZG](https://www.st.com/en/evaluation-tools/nucleo-f746zg.html)
- STM32F7ZG examples use [the NUCLEO-F746ZG board](https://os.mbed.com/platforms/ST-Nucleo-F746ZG/)

# Repository Structure

- Baremetal
  - Low level Linker script and Learning
  - Integrate critical external components to a project
- doc
  - PDF Documents
- STM32_HAL
  - STM32 HAL based examples for various microcontrollers
- Template
  - Projects built after `Minimal_CMSIS` in Baremetal
  - Actual application level code
- third_party
  - Third Party software ZIP files
- tools
  - Tools used by the project
  - Links or ZIP files

# Project Structure

**AIM:** Create a folder structure can would need minimal change when porting between different architectures and microcontrollers

In this below structure to port our project to a different controller we would need to update these folders
- l0_device_specific
- l1_third_party_device_specific
- l2_drivers

```
+-- l0_device_specific
| +-- arm_cmsis
| +-- linker_script
| +-- device_headers
| +-- device_startup
| +-- toolchain_specific_syscalls
+-- l1_third_party_device_specific
| +-- RTOS
| +-- External_Libraries_device_specific
+-- l2_drivers
| +-- gpio_device_specific
| +-- uart_device_specific
+-- l3_board_specific
| +-- peripheral_initialization
+-- l3_functional_third_party
| +-- ring_buffer
+-- l4_user_module
| +-- external_hardware
+-- l5_application
| +-- application_logic
| +-- main.c
```

## Device Dependent folders

- l0_device_specific
- l1_third_party_device_specific
- l2_drivers

### l0_device_specific

- ARM CMSIS 5 has different compiler and architecture specific changes
- ARM CMSIS 5 Linker script is dependent on architecture of microcontroller
- Device Header is dependent of the Microcontroller Manufacturer
- Device Startup is dependent on ARM Architecture and Microcontroller Manufacturer

### l1_third_party_device_dependent

- Certain Third Party software changes its behaviour based on the architecture and device
- For example: FreeRTOS needs to be configured differently according to different microcontroller family and architectures

### l2_drivers

- Basic drivers for **GPIO**, **Interrupt Handing**
- Basic protocol for **UART**, **SPI**, **I2C**
- These are microcontroller and architecture dependent

## Device Independent folders

- l3_functional_third_party
- l4_user_module
- l5_application

### l3_board_specific

- For internal board_specific initialization
- Syscalls based externed functions

### l3_functional_third_party

- Functional Third Party code integrated into the project
- For example: **Ring Buffer**, **JSON Library**

### l4_user_module

Writing code for various hardwares i.e

- Communication
- Sensors
- Actuators

### l5_application

- Application Logic for the project
- `main.c` resides on the top level

# Roadmap

- Understanding your ARM Toolchain
  - [x] arm-none-eabi-*
  - [ ] clang for ARM
- Cross Compilation with Buildsystems
  - [x] CMake
  - [x] Meson
  - [ ] Bazel
  - [ ] Premake
- ARM Linker Script
  - [x] Custom Linker Script
  - [x] Custom Startup
  - [x] Integrate CMSIS Linker Script
  - [x] Integrated CMSIS Startup
- Platform
  - [x] CMSIS_5
  - [x] STM32 HAL
- RTOS
  - [x] Amazon FreeRTOS
  - [ ] Microsoft ThreadX
- Languages
  - [x] C
  - [ ] C++
  - [ ] Rust
- Debugging
  - [x] openOCD
  - [x] `arm-none-eabi-gdb`
  - [x] Semihosting
- Unit-Test and Mocking
  - [ ] Unity
  - [ ] CppUTest
  - [ ] Google Tests
  - [ ] Catch
- Third Party Modules
  - [ ] Ring Buffer
  - [ ] JSON Parser
- Tool Integration
  - [ ] Clang Format
  - [ ] Static Analysis
  - [ ] Code Coverage
  - [ ] Continuous Integration
  - [ ] Documentation Generator
