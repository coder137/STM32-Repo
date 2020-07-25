cmake_minimum_required(VERSION 3.16)

# Set your constants here
set(PROJECT_NAME "minimal_blinky")

# Project start
set(CMAKE_TOOLCHAIN_FILE "toolchain.cmake")
project(${PROJECT_NAME})

set(USER_PROJECT_TARGET "${PROJECT_NAME}.elf")
add_executable(${USER_PROJECT_TARGET} src/main.c)

# TODO, Add more files here

# TODO, Add more variables here

# Project Variables
set(COMMON_C_FLAGS
    -O0
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
    # _scanf_float"
    -nostdlib
    -static
    # -Wl,--gc-sections
    -Wl,-Map,${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
    -T${PROJECT_SOURCE_DIR}/linker/layout_stm32l4.ld)

# ? debugging
message("\nDEBUGGING")
message("COMMON_FLAGS: ${COMMON_C_FLAGS}")
message("USER_COMPILE_C_FLAGS: ${USER_C_FLAGS}")
message("USER_LINK_FLAGS: ${USER_LINK_FLAGS}")
message("-------DONE------\n\n")

target_compile_options(${USER_PROJECT_TARGET} PRIVATE ${COMMON_C_FLAGS}
                                                      ${USER_C_FLAGS})
target_link_options(${USER_PROJECT_TARGET} PRIVATE ${COMMON_C_FLAGS}
                    ${USER_LINK_FLAGS})
target_include_directories(${USER_PROJECT_TARGET} PRIVATE "startup/cmsis"
                                                          "startup/device")

set(TOOLS_ARM_EABI_NONE_SIZE "arm-none-eabi-size")
add_custom_command(
  TARGET ${USER_PROJECT_TARGET}
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf
          ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin
  COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf
          ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex
  COMMAND
    ${CMAKE_OBJDUMP} --source --all-headers --demangle --line-numbers --wide
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf >
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.lst
  COMMAND ${TOOLS_ARM_EABI_NONE_SIZE} --format=berkeley
          ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf
  VERBATIM USES_TERMINAL)

# TODO, Shift this to a new .cmake file Custom Target to Flash Hardware
set(ST_PROGRAMMER "STM32_Programmer_CLI.exe")
add_custom_target(
  find_device
  COMMAND ${ST_PROGRAMMER} -l st-link
  VERBATIM USES_TERMINAL)

add_custom_target(
  flash
  DEPENDS ${USER_PROJECT_TARGET}
  COMMAND ${ST_PROGRAMMER} -c port=SWD sn=066CFF323338424E43146025 -w
          ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin 0x08000000
  VERBATIM USES_TERMINAL)
