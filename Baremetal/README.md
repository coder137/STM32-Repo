# List

[Install the arm-none-eabi-* toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
[Install CMAKE](https://cmake.org/cmake/help/latest/index.html)
[Fastbit Embedded Brain Academy Linker Script](https://www.youtube.com/watch?v=B7oKdUvRhQQ)

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

## C Programming

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
- [ ] Minimal_Startup
  - Using the Minimal Blinky to write startup files 
- [ ] Minimal_CMSIS
  - Using the ARM CMSIS Base Toolchain 
  - Device specific data added
- [ ] Minimal_C_Stdlib
  -  Adding the C Standard library
  -  Using syscalls implementation

## C++ Programming

- [ ] Minimal_Linker_Cpp
  - Adding C++
  - Constructor, Destructor
  - Templates
  - C++11 Features
- [ ] Minimal_Linker_Cpp_Stdlib
  - Using the C++ Standard libraries
  - `<algorithm>` and static allocation based functions
- [ ] Minimal_Linker_RTOS
  - Adding FreeRTOS to the device
  - Writing device specific FreeRTOS.h files