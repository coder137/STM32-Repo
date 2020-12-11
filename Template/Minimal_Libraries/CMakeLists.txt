cmake_minimum_required(VERSION 3.16)

# Set your constants inside this file
include(options.cmake)

if (NOT TESTING)
  set(CMAKE_TOOLCHAIN_FILE "cmake/toolchain.cmake")
endif()

project(${PROJECT_NAME})

# CMake Settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CMake functions
include(cmake/function.cmake)
include(cmake/test/target.cmake)

# Cross Compilation Flags
include(cmake/compile_flags.cmake)

set(USER_INCLUDE_DIRS "")

# Set Global requirements for both unit-tests and arm targets
add_compile_definitions(${CONTROLLER_NAME})

# NOTE!, Testing does not work when adding static libraries
# We need to manually populate the above USER_INCLUDE_DIRS to mocks and unit-tests
if (TESTING)

  message("Configuring Testing")
  
  include(CTest)
  enable_testing()

  # Unit Testing libraries
  include(cmake/testing.cmake)

  # TODO, Add more mocks here

  # TODO, Update this w.r.t the mocks above
  set(USER_TEST_DIRS 
      ${USER_INCLUDE_DIRS}
      ${rcc_mock_include_dirs}
      ${gpio_mock_include_dirs}
  )

  # TODO, Add more tests here

  # TODO, Add code coverage tool

else()

  message("Configuring ARM Project")

  # Each layer as libraries
  # l0_lowlevel
  add_subdirectory(l0_lowlevel/arm)
  add_subdirectory(l0_lowlevel/device/${CONTROLLER_NAME})
  add_subdirectory(l0_lowlevel/newlib)

  # l1_third_party
  add_subdirectory(l1_third_party/FreeRTOS)

  # l2_layer
  add_subdirectory(l2_driver/${CONTROLLER_NAME})
  add_subdirectory(l2_peripheral/${CONTROLLER_NAME})

  # Cross Compiled project
  set(USER_PROJECT_TARGET "${PROJECT_NAME}.elf")
  add_executable(${USER_PROJECT_TARGET}
      l5_application/${CONTROLLER_NAME}_main.c
  )

  target_include_directories(${USER_PROJECT_TARGET} PRIVATE
     ${USER_INCLUDE_DIRS}
  )

  target_compile_options(${USER_PROJECT_TARGET} PRIVATE 
      ${COMMON_C_FLAGS}
      ${USER_C_FLAGS}
  )

  target_link_options(${USER_PROJECT_TARGET} PRIVATE 
    ${COMMON_C_FLAGS}
    ${USER_LINK_FLAGS}
  )

  target_link_libraries(${USER_PROJECT_TARGET} PRIVATE 
    arm_cmsis_core
    device
    newlib
    freertos
    driver
    peripheral
  )

  # Features
  include(cmake/cross/semihosting.cmake)

  # Tools
  include(cmake/compile.cmake)
  include(cmake/flash.cmake)
  # TODO, Add Static Analysis tool
  # TODO, Add documentation generator tool

endif()

# TODO, Add Clang format tool
