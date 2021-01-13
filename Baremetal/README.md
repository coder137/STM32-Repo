- [List](#list)
  - [High Level Overview of Baremetal Cross Compilation Process](#high-level-overview-of-baremetal-cross-compilation-process)
  - [Important](#important)
    - [**dump** folder](#dump-folder)
- [Projects](#projects)
  - [Embedded C](#embedded-c)
  - [Embedded C++](#embedded-c-1)
  - [Embedded Rust](#embedded-rust)

# List

Examples have been built on the **B-L475E-IOT01A STM32L4 based board**

- [Install the arm-none-eabi-* toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
- [Install CMAKE](https://cmake.org/cmake/help/latest/index.html)
- [Fastbit Embedded Brain Academy Linker Script](https://www.youtube.com/watch?v=B7oKdUvRhQQ)
- [ARM CMSIS_5 Github](https://github.com/ARM-software/CMSIS_5)
- [STM32 Github](https://github.com/STMicroelectronics/STM32Cube_MCU_Overall_Offer)

## High Level Overview of Baremetal Cross Compilation Process

- Write / Get the linker script
  - Support for `.text`, `.data` and `.bss` sections
  - Support for C Standard Library
  - Support for C++ Standard Library
- Write the Startup File (CMSIS)
  - Initialize the ISR Vector Table
  - Set the Stack Pointer at the first address
  - Implement the Reset Handler
- Implement the Reset Handler
  - Copy the `.data` section from Flash to RAM (LMA to VMA)
  - Initialize the `.bss` section to 0
    - Data initialized to 0 is stored here
    - Data uninitialized also needs to be set to 0 and stored here
- Call the main service routine

## Important

### **dump** folder
  - contains various compilation dumps to learn the various flags that are used during compilation

# Projects

## Embedded C

- [x] Minimal_Linker
  - Basic understanding of Linker Scripts
  - Basic C Programming used 
- [x] Minimal_Linker_CMake
  - Integrating the Basic example with CMake Cross compilation toolchain 
- [x] Minimal_Blinky
  - Complete the Linker Script (base) with ALIGN keyword as well
  - Very minimal Blinky example (NOT Complete)
  - Does not contain complete startup, .bss implementation and copy functions
  - Can be flashed onto the microcontroller to see led blinking
- [x] Minimal_Blinky_Meson
  - Integrated the Meson build system
  - Forked from Minimal_Blinky example 
- [x] Minimal_Startup
  - Using the Minimal Blinky to write startup files for your linker script
- [x] Minimal_CMSIS
  - Using the ARM CMSIS Base Toolchain 
  - Device specific data added

## Embedded C++

- [x] Minimal_Cpp
  - Adding C++17 support to Linker script
  - Functions vs Namespaced functions
  - Classes vs Structs
    - Constructor, Destructor code size considerations
  - Virtual Classes vs Templates
  - Custom flags for code size optimzation
- [ ] Minimal_Cpp_Stdlib
  - Dynamic memory allocation 
  - Standard C++ library and syscalls
- [ ] Minimal_Cpp_Advanced
  - C++11/C++14/C++17 static library features

## Embedded Rust

- [x] Minimal_Baremetal
  - Uses C FFI with rust (CMSIS and Device Headers)
  - No external crate support
  - Interrupt Vector Table in rust
  - Similar to `Minimal_CMSIS` C project
- [ ] Minimal_Tools
  - Basic inbuilt libraries and crates for embedded development
  - Basic tools for your embedded workflow
