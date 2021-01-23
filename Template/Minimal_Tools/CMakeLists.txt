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

# TODO, Add more subdirectory includes here
ess_include(l0_lowlevel)
ess_include_script(l1_third_party/FreeRTOS/FreeRTOS.cmake)
ess_include(l2_driver)
ess_include(l2_peripheral)
ess_include(l5_application)

set(USER_INCLUDE_DIRS 
  ${l0_lowlevel_dirs}
  ${FreeRTOS_dirs}
  ${l2_driver_dirs}
  ${l2_peripheral_dirs}
  ${l5_application_dirs}
)

set(USER_SOURCES
  ${l0_lowlevel_sources}
  ${FreeRTOS_sources}
  ${l2_driver_sources}
  ${l2_peripheral_sources}
  ${l5_application_sources}
)

set(USER_CLANG_SOURCES
  ${l2_driver_sources}
  ${l2_peripheral_sources}
  ${l5_application_sources}
)

set(USER_CLANG_INC_DIRS
  ${l2_driver_dirs}
  ${l2_peripheral_dirs}
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

  # TODO, Update this w.r.t the mocks above
  set(USER_TEST_DIRS 
      ${USER_INCLUDE_DIRS}
      ${rcc_mock_include_dirs}
      ${gpio_mock_include_dirs}
  )

  # TODO, Add more tests here

  # NOTE, Append to USER_CLANG_SOURCES and USER_CLANG_INC_DIRS
  # to add unit-tests to clang format

  # TODO, Add code coverage tool

else()

  message("Configuring ARM Project")

  # Tools to add before target is defined
  ## Static analysis tool
  include(cmake/tool/cppcheck.cmake)

  # Cross Compiled project
  include(cmake/project/project_release.cmake)
  # TODO, Add more projects here

  # Features
  include(cmake/cross/semihosting.cmake)

  # Tools
  include(cmake/compile.cmake)
  include(cmake/flash.cmake)
  # TODO, Add documentation generator tool

endif()

# TODO, Add Clang format tool
include(cmake/tool/clangformat.cmake)
