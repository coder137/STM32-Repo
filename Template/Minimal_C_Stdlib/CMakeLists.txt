cmake_minimum_required(VERSION 3.16)

# Set your constants here
set(PROJECT_NAME "Minimal_C_Stdlib")
set(CONTROLLER_NAME "STM32L475xx")
set(USE_SEMIHOSTING ON)

# Project start
set(CMAKE_TOOLCHAIN_FILE "cmake/toolchain.cmake")
project(${PROJECT_NAME})

set(USER_PROJECT_TARGET "${PROJECT_NAME}.elf")
add_executable(${USER_PROJECT_TARGET} application/main.c)

target_compile_definitions(${USER_PROJECT_TARGET} PRIVATE ${CONTROLLER_NAME})
target_compile_options(${USER_PROJECT_TARGET} PRIVATE ${COMMON_C_FLAGS}
                                                      ${USER_C_FLAGS})

if(USE_SEMIHOSTING)
  message("Configuring Project with Semihosting")
  target_link_options(${USER_PROJECT_TARGET} PRIVATE 
  ${COMMON_C_FLAGS}
  ${USER_SEMIHOSTING_FLAGS}
  ${USER_LINK_FLAGS}
  )
  target_compile_definitions(${USER_PROJECT_TARGET} PRIVATE SEMIHOSTING=1)
else()
  message("Turning Semihosting off")
  target_link_options(${USER_PROJECT_TARGET} PRIVATE 
    ${COMMON_C_FLAGS}
    ${USER_NO_SEMIHOSTING_FLAGS}
    ${USER_LINK_FLAGS}
  )
endif()

# TODO, Add more subdirectories here
add_subdirectory(l0_lowlevel)
add_subdirectory(driver)
add_subdirectory(modules)
add_subdirectory(third_party/FreeRTOS)

# Tools
include(cmake/compile.cmake)
