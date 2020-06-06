# Project Settings
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Project Variables
set(COMMON_C_FLAGS
    -Os
    -mcpu=cortex-m4
    -mthumb
    -mfloat-abi=hard
    -mfpu=auto
    -Wall
    -g
    -pipe)
set(USER_C_FLAGS -std=c99 -fmessage-length=0 -ffunction-sections
                 -fdata-sections -fstack-usage)

set(USER_LINK_FLAGS
    -nostartfiles
    # -specs=nano.specs -specs=nosys.specs "SHELL:-u _printf_float" "SHELL:-u
    # _scanf_float" -nostdlib
    -static
    -Wl,--gc-sections
    -Wl,-Map,${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
    -T${PROJECT_SOURCE_DIR}/l0_lowlevel/linker/gcc_arm.ld)
