- [Minimal CMSIS](#minimal-cmsis)
- [Current Links](#current-links)
- [[NEW] Integration with CMSIS](#new-integration-with-cmsis)
  - [High Level Overview](#high-level-overview)
  - [Technical Overview](#technical-overview)
- [OLDER](#older)
- [Pre-requisite](#pre-requisite)
- [Integrating with CMake](#integrating-with-cmake)
  - [NOTE](#note)
- [Adding File dependencies](#adding-file-dependencies)
  - [Added the **STM32 Device Files**](#added-the-stm32-device-files)
  - [Adding the CMSIS Files](#adding-the-cmsis-files)
  - [Update CMake](#update-cmake)
- [Baremetal Blink](#baremetal-blink)
- [Flashing](#flashing)
  - [Flashing from Command Line](#flashing-from-command-line)
  - [Flashing from CMake](#flashing-from-cmake)

# Minimal CMSIS

AIM: Integrate the ARM CMSIS stack with the STM32 Device files

> Copied from Minimal Startup

# Current Links

- [ARM CMSIS](https://developer.arm.com/tools-and-software/embedded/cmsis)

# [NEW] Integration with CMSIS

## High Level Overview

- Get the ARM CMSIS **Cortex M Includes**
  - No Configuration required
- Get the ARM Device **C Startup File** and **Linker Script**
  - C Startup File requires Hardware specific includes
  - ARM Linker Script requires ROM, RAM based configurations
- Get the Hardware specific files from **STM32 Device Repository**
  - No Configuration required
- Write your **Entry Point C File** and call `main`
- Write your application code in `main`

## Technical Overview

- ARM Folder contains CMSIS Cortex M Drivers
  - These can be obtained from [the CMSIS Github Repository](https://github.com/ARM-software/CMSIS_5)
  - Take a look under `Core/Include`
- Copy the **Linker Script** from the Device folder
  - `CMSIS_5/Device/ARM/ARMCM4/Source/GCC/gcc_arm.ld`
  - **IMP** Make sure you initialize the below mentioned variables
    - **__ROM_BASE**, **__ROM_SIZE** (Flash size of your controller)
    - **__RAM_BASE**, **__RAM_SIZE** (RAM size of your controller)
    - **__STACK_SIZE**, **__HEAP_SIZE** (Stack and Heap size of your controller)
- We also need our device specific startup code.
  - We get this from [the STM32 Github Repository](https://github.com/STMicroelectronics/STM32Cube_MCU_Overall_Offer)
  - Take a look under `Drivers/CMSIS/Device`
  - Files to copy
    - Sources `system_stm32l4xx`
    - Include Folder `Include`
- The `startup_stm32l4xx.c` is startup file from the **ARM CMSIS_5 Device Folder**
    - `CMSIS_5/Device/ARM/ARMCM4/Source/startup_ARMCM4.c`
    - Copy and rename this file
    - Delete the initial includes and add the stm32 specific includes here

- Entry Point inside **startup_stm32l4xx.c**
  - Inside the `Reset_Handler` function there is a macro function called `__PROGRAM_START`
  - `__PROGRAM_START` internally initializes the bss and copies the data section
  - It also calls the `extern void _start(void)` function
  - This needs to be defined by us and what runs after the Low Level has been completed
- Create a file `entry_point.c` and add this above function `void _start(void)`
  - Inside this function put your call to `int main()`


# OLDER

Relevant Documentation and Links from previous projects 

# Pre-requisite

- [Read this amazing tutorial](https://interrupt.memfault.com/blog/how-to-write-linker-scripts-for-firmware)
- [Read the STM32 getting started from scratch guide](http://tty.uchuujin.de/2016/02/stm32-from-scratch-bare-minimals/)
- [Github Link to the same](https://github.com/textshell/stm32-from-scratch)

# Integrating with CMake

- [CMake Documentation](https://cmake.org/cmake/help/latest/index.html)
- [Cross Compilation](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html)
- Install the latest `arm-none-eabi-*` toolchain
  - Tested with `9.2.1`

```bash
# Raw command run
cmake.exe --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug "-DCMAKE_C_COMPILER:FILEPATH=D:\Software\GNU Tools Arm Embedded\9 2019-q4-major\bin\arm-none-eabi-gcc.exe" "-DCMAKE_CXX_COMPILER:FILEPATH=D:\Software\GNU Tools Arm Embedded\9 2019-q4-major\bin\arm-none-eabi-g++.exe" -Hd:/Repositories/STM32-Repo/Baremetal/Minimal_Linker_CMake -Bd:/Repositories/STM32-Repo/Baremetal/Minimal_Linker_CMake/build -G Ninja
```

- Add your **COMPILE_FLAGS** using the `target_compile_options`
- Add your **LINK_FLAGS** using the `target_link_options`
- Convert your **.elf** generated to other format by running custom commands
  - Use the `add_custom_command` option
- After the project is generated we can build using the command
  - `cmake --build build --clean-first -- -v`
  - Ninja gives a very good verbose output

## NOTE

- Inside `toolchain.cmake` we need to set
  - `set(CMAKE_C_COMPILER_WORKS 1)`
  - `set(CMAKE_CXX_COMPILER_WORKS 1)`
  - This will ignore the program check for the compilers


# Adding File dependencies

Adding CMSIS and STM32 Device Files

## Added the **STM32 Device Files**

Download the STM32L4 packages from [the STM32 Website](https://www.st.com/en/embedded-software/stm32cubel4.html)

- Find the Device Packages under `Drivers/CMSIS/Device/ST/STM32L4xx/Include` and pick your appropriate files
  - In my case `STM32l475xx.h`
  - We also have to take the `system_stm32l4xx.h` (Include dependency for the above)

## Adding the CMSIS Files

For the sake of this project we will take the CMSIS Files from the STM32 Software Pack

- Find the CMSIS Package under `Drivers/CMSIS/Core/Include`
  - Copy all of the files and add them to your `startup/cmsis` folder

## Update CMake

Update your includes by using
```cmake
target_include_directories(${USER_PROJECT_TARGET} PRIVATE "startup/cmsis" "startup/device")
```

# Baremetal Blink

See the [DISCO-L475VG-IOT01A Pin Layout](https://os.mbed.com/platforms/ST-Discovery-L475E-IOT01A/)

Inside the **doc folder** we use the **STM32L4x5 Reference Manual** to understand the usage of Registers

- `RCC` Register
  - Power the `GPIOA` Register
- `GPIO` Register
  - `MODER`
  - `OTYPER`
  - `OSPEEDR`
  - `PUPDR`
  - `BRR`
  - `BSRR`
- Using `__NOP`
  - Assembly instruction for `nop`
  - Burning CPU cycles during delay

# Flashing

Download the [STM32Prog Tool](https://www.st.com/en/development-tools/stm32cubeprog.html)

## Flashing from Command Line

```bash
# Help
STM32_Programmer_CLI.exe --help

# Find ST-LINK based devices
STM32_Programmer_CLI.exe -l st-link

# Flash the code
# Connect and then write to appropriate location
STM32_Programmer_CLI.exe -c port=SWD sn=<st-link sn> -w <project.bin> 0x08000000
```

## Flashing from CMake

```cmake
set(ST_PROGRAMMER "STM32_Programmer_CLI.exe")
add_custom_target(
  find_device
  COMMAND ${ST_PROGRAMMER} -l st-link
  VERBATIM USES_TERMINAL)

add_custom_target(
  flash
  DEPENDS ${USER_PROJECT_TARGET}
  COMMAND STM32_Programmer_CLI.exe -c port=SWD sn=066CFF323338424E43146025 -w
          ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin 0x08000000
  VERBATIM USES_TERMINAL)
```

```bash
# Find Device
cmake --build build --target find_device

# Flash Device
cmake --build build --target flash
```
