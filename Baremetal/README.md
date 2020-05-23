# List

[Install the arm-none-eabi-* toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
[Install CMAKE](https://cmake.org/cmake/help/latest/index.html)
[Fastbit Embedded Brain Academy Linker Script](https://www.youtube.com/watch?v=B7oKdUvRhQQ)

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
- [ ] Minimal_Linker_Startup
  - Using the Minimal Blinky to write startup files 
- [ ] Minimal_Linker_CMSIS
  - Using the ARM CMSIS Base Toolchain 
  - Device specific data added
- [ ] Minimal_Linker_C_Stdlib
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
