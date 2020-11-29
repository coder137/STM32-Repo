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
  VERBATIM USES_TERMINAL)

add_custom_target(size ALL
  DEPENDS ${USER_PROJECT_TARGET}
  COMMAND ${TOOLS_ARM_EABI_NONE_SIZE} --format=berkeley ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf
  VERBATIM USES_TERMINAL)

# TODO, Shift these to its own file
add_custom_target(
  openocd_start_server
  DEPENDS ${USER_PROJECT_TARGET}
  COMMAND openocd.exe -f ${PROJECT_SOURCE_DIR}/stm32l4discovery.cfg
  VERBATIM USES_TERMINAL)

add_custom_target(
  openocd_connect_client
  DEPENDS ${USER_PROJECT_TARGET}
  COMMAND arm-none-eabi-gdb.exe ${PROJECT_BINARY_DIR}/${USER_PROJECT_TARGET}
  VERBATIM USES_TERMINAL)
