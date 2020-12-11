add_library(driver STATIC 
  exti/exti.c
  rcc/rcc.c
  gpio/gpio.c
  uart/uart.c
  uart/uart_interrupt.c
)

target_include_directories(driver PUBLIC 
  "."
  "../"
)

target_compile_options(driver PUBLIC 
      ${COMMON_C_FLAGS}
      ${USER_C_FLAGS}
)

target_link_options(driver PUBLIC 
  ${COMMON_C_FLAGS}
  ${USER_LINK_FLAGS}
)

target_link_libraries(driver PRIVATE 
  arm_cmsis_core
  device
  freertos
)
