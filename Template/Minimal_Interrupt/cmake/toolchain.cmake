# Project Settings
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Make sure your arm-none-eabi- toolchain is added to PATH
# https://developer.arm.com/tools-and-software/open-source-software/developer-
# tools/gnu-toolchain/gnu-rm/downloads
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# https://gcc.gnu.org/onlinedocs/gcc-3.2/gcc/Warning-
# Options.html#Warning%20Options
set(WARNING_FLAGS -Wall -Werror)

# Project Variables
set(COMMON_C_FLAGS
    -Os
    -mcpu=cortex-m4
    -mthumb
    -mfloat-abi=hard
    -mfpu=auto
    -g
    -pipe
    ${WARNING_FLAGS})
set(USER_C_FLAGS -std=c99 -fmessage-length=0 -ffunction-sections
                 -fdata-sections -fstack-usage)

set(USER_LINK_FLAGS
    -nostartfiles
    -specs=nano.specs
    -specs=nosys.specs
    # "SHELL:-u _printf_float" "SHELL:-u _scanf_float" -nostdlib
    -static
    -Wl,--gc-sections
    -Wl,-Map,${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
    -T${PROJECT_SOURCE_DIR}/l0_lowlevel/linker/gcc_arm.ld)
