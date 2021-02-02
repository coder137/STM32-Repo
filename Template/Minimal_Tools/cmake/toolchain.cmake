# Project Settings
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# ~~~
# Make sure your arm-none-eabi- toolchain is added to PATH
# https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
# ~~~
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# This is necessary to populate `CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES`
# a lot of tools could potentitally require cmake variable
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
