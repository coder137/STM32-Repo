# Unity Compilation
add_library(
  unity STATIC testing/Unity/unity.c
)

target_include_directories(unity PUBLIC 
        testing/Unity
)

# FFF Compilation
add_library(fff INTERFACE)
target_include_directories(fff INTERFACE testing)
