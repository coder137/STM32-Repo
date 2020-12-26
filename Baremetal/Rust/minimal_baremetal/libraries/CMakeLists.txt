cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "toolchain.cmake")
project(c_libraries LANGUAGES C)

# Device library
add_library(device STATIC)
target_sources(device PUBLIC device/device.c)
target_include_directories(device PUBLIC 
  device
  device/cmsis
)

set(COMMON_C_FLAGS
  -mthumb
  -mfloat-abi=hard
  -march=armv7e-m 
  -mfpu=fpv4-sp-d16
)

target_compile_options(device PUBLIC 
  -ffunction-sections 
  -fdata-sections 
  ${COMMON_C_FLAGS}
)

target_link_options(device PUBLIC
  ${COMMON_C_FLAGS}
  -nostartfiles
  -nostdlib
  -static
)

# TODO, Add more libraries
