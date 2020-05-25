# If a new target is added
# `startup_stm32l4xx.c` and `system_stm32l4xx.c` source files need to be added to the if condition
# target_include_directories also needs to be updated `stm32`
target_sources(
  ${USER_PROJECT_TARGET}
  PRIVATE "startup_stm32l4xx.c"
          "system_stm32l4xx.c")
target_include_directories(${USER_PROJECT_TARGET} PRIVATE "stm32")
