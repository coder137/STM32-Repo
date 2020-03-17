- [Minimal_Linker_CMake](#minimallinkercmake)
- [Pre-requisite understanding](#pre-requisite-understanding)
- [Integrating with CMake](#integrating-with-cmake)
- [NOTE](#note)

# Minimal_Linker_CMake

AIM: Integrating the Minimal Linker example with CMake to automate building

> Copied from Minimal Linker

# Pre-requisite understanding

- [CMake Documentation](https://cmake.org/cmake/help/latest/index.html)
- [Read this amazing tutorial](https://interrupt.memfault.com/blog/how-to-write-linker-scripts-for-firmware)
- [Read the STM32 getting started from scratch guide](http://tty.uchuujin.de/2016/02/stm32-from-scratch-bare-minimals/)
- [Github Link to the same](https://github.com/textshell/stm32-from-scratch)

# Integrating with CMake

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

# NOTE

- Inside `toolchain.cmake` we need to set
  - `set(CMAKE_C_COMPILER_WORKS 1)`
  - `set(CMAKE_CXX_COMPILER_WORKS 1)`
  - This will ignore the program check for the compilers
