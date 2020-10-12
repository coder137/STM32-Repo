# Configuration: https://www.freertos.org/Creating-a-new-FreeRTOS-project.html

# Add your FreeRTOS source
freertos_sources = files([
  'croutine.c',
  'event_groups.c',
  'list.c',
  'queue.c',
  'stream_buffer.c',
  'tasks.c',
  'timers.c'  
])

# ~~~
# include/ contains your FreeRTOS.h core includes
# config/ contains your FreeRTOSConfig.h config file
# FreeRTOSConfig.h copied from Demo/CORTEX_M4F_STM32F407ZG-SK
# ~~~
freertos_inc_dirs = []
freertos_inc_dirs += include_directories([
  'include',
  'config'
])

# ~~~
# [PORT SPECIFIC] GCC Compiler, ARM_CM4F Architecture This also contains
# ARM_CM4_MPU (Memory Protection Unit)
# ~~~
freertos_sources += files([
  'portable/GCC/ARM_CM4F/port.c'
])
freertos_inc_dirs += include_directories(['portable/GCC/ARM_CM4F'])

# HEAP Selection https://www.freertos.org/a00111.html
freertos_sources += files([
  'portable/MemMang/heap_4.c'
])
