# TODO, Add more platforms and their flashers here
if (${CONTROLLER_NAME} STREQUAL "STM32L475xx")
  include(cmake/flash/stm32_flash.cmake)
elseif(${CONTROLLER_NAME} STREQUAL "LPC4078xx")
  include(cmake/flash/lpc_flash.cmake)
endif()
