add_library(peripheral STATIC 
  peripheral.c
)

target_include_directories(peripheral PUBLIC 
  "."
)

target_compile_options(peripheral PUBLIC 
      ${COMMON_C_FLAGS}
      ${USER_C_FLAGS}
)

target_link_options(peripheral PUBLIC 
  ${COMMON_C_FLAGS}
  ${USER_LINK_FLAGS}
)

target_link_libraries(peripheral PRIVATE
  arm_cmsis_core
  device
  freertos
  driver
)
