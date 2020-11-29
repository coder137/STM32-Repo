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

# TODO, Add more subdirectory includes here
ess_include(l0_lowlevel)
# ess_include_script(l1_third_party/FreeRTOS/FreeRTOS.cmake)
ess_include(l2_driver)
# ess_include(l2_peripheral)
# ess_include(l5_application)

set(USER_INCLUDE_DIRS 
  ${l0_lowlevel_dirs}
  ${l2_driver_dirs}
  ${l2_peripheral_dirs}
  ${FreeRTOS_dirs}
  ${l5_application_dirs}
)

# Set Global requirements for both unit-tests and arm targets
add_compile_definitions(${CONTROLLER_NAME})

if (TESTING)

  message("Configuring Testing")
  
  include(CTest)
  enable_testing()

  # Unit Testing libraries
  include(cmake/testing.cmake)

  # TODO, Add more mocks here
  include(l2_driver/${CONTROLLER_NAME}/rcc/mock/include.cmake)
  include(l2_driver/${CONTROLLER_NAME}/gpio/mock/include.cmake)

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

  # Cross Compilation Flags
  include(cmake/compile_flags.cmake)

  # Cross Compiled project
  set(USER_PROJECT_TARGET "${PROJECT_NAME}.elf")
  add_executable(${USER_PROJECT_TARGET}
      ${l0_lowlevel_sources}
      ${FreeRTOS_sources}
      ${l2_driver_sources}
      ${l2_peripheral_sources}
      ${l5_application_sources}
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

  # Features
  include(cmake/cross/semihosting.cmake)

  # Tools
  include(cmake/compile.cmake)
  include(cmake/flash.cmake)
  # TODO, Add Static Analysis tool
  # TODO, Add documentation generator tool

endif()

# TODO, Add Clang format tool
