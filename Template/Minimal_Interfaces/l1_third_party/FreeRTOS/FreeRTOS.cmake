# Configuration: https://www.freertos.org/Creating-a-new-FreeRTOS-project.html

set(FreeRTOS_sources "")
set(FreeRTOS_dirs "")

# Add your FreeRTOS source
ess_data(FreeRTOS_sources 
    croutine.c
    event_groups.c
    list.c
    queue.c
    stream_buffer.c
    tasks.c
    timers.c
)

# ~~~
# include/ contains your FreeRTOS.h core includes
# config/ contains your FreeRTOSConfig.h config file
# FreeRTOSConfig.h copied from Demo/CORTEX_M4F_STM32F407ZG-SK
# ~~~
ess_data(FreeRTOS_dirs
    include
    config
)

# ~~~
# [PORT SPECIFIC] GCC Compiler, ARM_CM4F Architecture This also contains
# ARM_CM4_MPU (Memory Protection Unit)
# ~~~
ess_data(FreeRTOS_sources
    portable/GCC/ARM_CM4F/port.c
)
ess_data(FreeRTOS_dirs
    portable/GCC/ARM_CM4F
)

# HEAP Selection https://www.freertos.org/a00111.html
ess_data(FreeRTOS_sources
    portable/MemMang/heap_4.c
)
