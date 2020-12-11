add_library(newlib STATIC 
  syssbrk.c
  syscalls.c
)

target_include_directories(newlib PUBLIC .)
target_compile_options(newlib PUBLIC 
      ${COMMON_C_FLAGS}
      ${USER_C_FLAGS}
)

target_link_options(newlib PUBLIC 
  ${COMMON_C_FLAGS}
  ${USER_LINK_FLAGS}
)
