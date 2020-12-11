add_library(freertos STATIC 
  croutine.c
  event_groups.c
  list.c
  queue.c
  stream_buffer.c
  tasks.c
  timers.c
  # Config dependent
  portable/GCC/ARM_CM4F/port.c
  portable/MemMang/heap_4.c
)

target_include_directories(freertos PUBLIC 
  include
  config/${CONTROLLER_NAME}
  # Config dependent
  portable/GCC/ARM_CM4F
)

target_compile_options(freertos PUBLIC 
      ${COMMON_C_FLAGS}
      ${USER_C_FLAGS}
)

target_link_options(freertos PUBLIC 
  ${COMMON_C_FLAGS}
  ${USER_LINK_FLAGS}
)

target_link_libraries(freertos PRIVATE
  arm_cmsis_core
  device
)
