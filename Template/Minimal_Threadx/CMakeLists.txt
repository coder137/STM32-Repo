cmake_minimum_required(VERSION 3.16)

# Set your constants here
set(PROJECT_NAME "Minimal_Interrupt")
set(CONTROLLER_NAME "STM32L475xx")

# Project start
set(CMAKE_TOOLCHAIN_FILE "cmake/toolchain.cmake")
project(${PROJECT_NAME}
  LANGUAGES C ASM
)

add_subdirectory(threadx)

set(USER_PROJECT_TARGET "${PROJECT_NAME}.elf")
add_executable(${USER_PROJECT_TARGET} 
  application/main.c
)

target_compile_definitions(${USER_PROJECT_TARGET} PRIVATE ${CONTROLLER_NAME})
target_compile_options(${USER_PROJECT_TARGET} PRIVATE ${COMMON_C_FLAGS}
                                                      ${USER_C_FLAGS})
target_link_options(${USER_PROJECT_TARGET} PRIVATE ${COMMON_C_FLAGS}
                    ${USER_LINK_FLAGS})
target_link_libraries(${USER_PROJECT_TARGET} PRIVATE threadx)

# TODO, Add more subdirectories here
add_subdirectory(l0_lowlevel)
add_subdirectory(modules)

# Tools
include(cmake/compile.cmake)
