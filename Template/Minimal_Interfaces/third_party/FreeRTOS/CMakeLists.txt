# Configuration: https://www.freertos.org/Creating-a-new-FreeRTOS-project.html

# Add your FreeRTOS source
target_sources(
  ${USER_PROJECT_TARGET}
  PRIVATE croutine.c
          event_groups.c
          list.c
          queue.c
          stream_buffer.c
          tasks.c
          timers.c)

# ~~~
# include/ contains your FreeRTOS.h core includes
# config/ contains your FreeRTOSConfig.h config file
# FreeRTOSConfig.h copied from Demo/CORTEX_M4F_STM32F407ZG-SK
# ~~~
target_include_directories(${USER_PROJECT_TARGET} PRIVATE include/ config/)

# ~~~
# [PORT SPECIFIC] GCC Compiler, ARM_CM4F Architecture This also contains
# ARM_CM4_MPU (Memory Protection Unit)
# ~~~
target_sources(${USER_PROJECT_TARGET} PRIVATE portable/GCC/ARM_CM4F/port.c)
target_include_directories(${USER_PROJECT_TARGET}
                           PRIVATE portable/GCC/ARM_CM4F/)

# HEAP Selection https://www.freertos.org/a00111.html
target_sources(${USER_PROJECT_TARGET} PRIVATE portable/MemMang/heap_4.c)
