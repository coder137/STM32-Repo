- [Integrate_FreeRTOS](#integrate_freertos)
- [Functional Explanation](#functional-explanation)
- [Technical Explanation](#technical-explanation)
  - [Downloading the Source](#downloading-the-source)
  - [Segregating the Source](#segregating-the-source)
  - [Add FreeRTOS Kernel files](#add-freertos-kernel-files)
  - [Updating FreeRTOSConfig](#updating-freertosconfig)
    - [Important Basic Macros](#important-basic-macros)
    - [Disabled Macros](#disabled-macros)
  - [ARM Reset Vector Updates](#arm-reset-vector-updates)
  - [Linker Script Considerations](#linker-script-considerations)

# Integrate_FreeRTOS

Integrating FreeRTOS with an ARM Toolchain

- [FreeRTOS steps for Custom Project](https://www.freertos.org/Creating-a-new-FreeRTOS-project.html)

# Functional Explanation

- Download the FreeRTOS Kernel and make sure these packages are added to your project
  - Source Files
  - Include Files for source
  - Compiler and Architecture specific Port (`portable` folder)
  - Memory Management (`MemMang` folder) i.e `heap_x.c`
  - `FreeRTOSConfig.h` file from Demo application
- Add this to your compilation unit
  - Added this to CMake build system
- Customize the `FreeRTOSConfig.h` file for your usages
  - Disable certain macros that have callback functions
  - > NOTE, Will be activated later after integration of C_Stdlib

# Technical Explanation

## Downloading the Source

Download the latest stable version of FreeRTOS from [Github via Tags ex. 10.3.1](https://github.com/FreeRTOS/FreeRTOS)

## Segregating the Source

Under **FreeRTOSv`<version>`/FreeRTOS/Source**

3 different types of files

- Source `.c` files
- `include` folder with `.h` files
- `portable` folder with Compiler and Architecture specific code

**Finally**
- `FreeRTOSConfig.h` from the **FreeRTOSv[version]/FreeRTOS/Demo** Folder
- [Supported Devices List](https://www.freertos.org/RTOS_ports.html)
  - You can also customize your ports as long as they have the same processor architecture
  - This will be discussed in detail later

## Add FreeRTOS Kernel files

**These files should be copied without being changed**

- Copy all Source Files
  - Minimum Source Files
  - `FreeRTOS/Source/tasks.c`
  - `FreeRTOS/Source/queue.c`
  - `FreeRTOS/Source/list.c`
- Copy the include Folder
  - Recommended to copy this entire folder
- Get your architecture `port`
  - Navigate to your compiler i.e `GCC`
  - Find you architecture i.e `ARM_CM4F`
    - There is also an `ARM_CM4_MPU` version of the same
    - FreeRTOS with integrated Memory Protection Unit
  - Copy that folder to your project
- Get your **memory management**
  - Copy `portable/MemMang/heap_x.c` to your project
  - Read this [Memory Management selection guide](https://www.freertos.org/a00111.html)
  - By default `heap_4.c` has been recommended by FreeRTOS
  - Kernel space is allocated on `.bss` section
  - Increases overall space but speed of allocation and freeing increases (more performant)

## Updating FreeRTOSConfig

- **This specifies the main behaviour of the Kernel on your ARM device**
- **You can and should modify this file as per your requirements**

### Important Basic Macros

- `configCPU_CLOCK_HZ`
  - Your ARM CPU speed
- `configTICK_RATE_HZ`
  - This tick rate of the Kernel.
  - 1000Hz is the most common speed set i.e 1 Tick == 1ms
- `configTOTAL_HEAP_SIZE`
  - Heap size allocated on the `.bss` section
  - This macros is only used when `heap_4` is selected
- `configPRIO_BITS`
  - The Priority Bits used by the ARM Microcontroller
  - Read the [FreeRTOS and NVIC Document](FreeRTOS_and_ARM_NVIC.md)
- [`configMAX_SYSCALL_INTERRUPT_PRIORITY`](https://www.freertos.org/a00110.html#kernel_priority)
  - **[IMPORTANT]** Read the above linked document
  - ISR at a higher priorty than this flag cannot be touched by the FreeRTOS kernel.
  - ISR at a lower priorty than this flag can be managed by the FreeRTOS kernel
    - Since ARM priority is inverse of its number. A higher number means a lower priorty
    - Since we want FreeRTOS kernel to manage our generic ISR functions we set the priorty to be `configMAX_SYSCALL_INTERRUPT_PRIORITY + 1` (to make it a lesser priority than the kernel)
    - See image below to understand

![configMAX_SYSCALL_INTERRUPT_PRIORITY](https://www.freertos.org/fr-content-src/uploads/2018/07/Interrupt-priorities-interrupt-nesting.jpg)


### Disabled Macros

- `configUSE_IDLE_HOOK`
- `configUSE_TICK_HOOK`
- `configCHECK_FOR_STACK_OVERFLOW`
- `configUSE_MALLOC_FAILED_HOOK`

> TODO, Understand what each of these macros does after integrating C_Stdlib


## ARM Reset Vector Updates

For the ARM FreeRTOS port we need to add 3 FreeRTOS specific ISR functions. 

[Check this code diff to add FreeRTOS Port functions to Reset Vector](https://github.com/coder137/STM32-Repo/commit/374ad15ef246a4fb895a09cae913bf297dae52ff#diff-e0a58ecbe8cbf63234930f64cb6c0295)

- `vPortSVCHandler` i.e Priority -5
- `xPortPendSVHandler` i.e Priority -2
- `xPortSysTickHandler` i.e Priorty -1

## Linker Script Considerations

- Make sure your Linker Script has enough space in the RAM for `.bss`, `.data` and the FreeRTOS kernel (also added to `.bss`)
- [Check this code diff for space calculations](https://github.com/coder137/STM32-Repo/commit/374ad15ef246a4fb895a09cae913bf297dae52ff#diff-12338f41b44c6f50db6da8955d9828ac)

