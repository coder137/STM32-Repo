target_sources(${USER_PROJECT_TARGET} PRIVATE "startup/isr_vector.c")
target_include_directories(${USER_PROJECT_TARGET} PRIVATE "startup/stm32")
