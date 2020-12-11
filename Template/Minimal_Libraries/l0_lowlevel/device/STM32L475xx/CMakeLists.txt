add_library(device STATIC 
  startup_stm32l4xx.c 
  system_stm32l4xx.c
  entry_point.c
)
target_include_directories(device PUBLIC .)
target_compile_options(device PUBLIC 
      ${COMMON_C_FLAGS}
      ${USER_C_FLAGS}
)

target_link_options(device PUBLIC 
  ${COMMON_C_FLAGS}
  ${USER_LINK_FLAGS}
)

target_link_libraries(device PRIVATE 
  arm_cmsis_core
)
