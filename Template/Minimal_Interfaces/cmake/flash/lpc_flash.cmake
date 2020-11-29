# TODO, Set COM Port in options file
set(LPC_PROGRAMMER "${CMAKE_SOURCE_DIR}/cmake/flash/nxpprog/nxpprog.py")
add_custom_target(
  flash
  DEPENDS ${USER_PROJECT_TARGET}
  COMMAND python ${LPC_PROGRAMMER} --device=COM3 --binary=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin
  VERBATIM USES_TERMINAL)
