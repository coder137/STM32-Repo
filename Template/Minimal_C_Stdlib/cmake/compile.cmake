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
