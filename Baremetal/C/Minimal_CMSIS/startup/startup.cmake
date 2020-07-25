target_sources(
  ${USER_PROJECT_TARGET}
  PRIVATE "startup/entry_point.c"
          # "startup/isr_vector.c"
          "startup/startup_stm32l4xx.c" "startup/system_stm32l4xx.c")
target_include_directories(${USER_PROJECT_TARGET} PRIVATE "startup/stm32")
