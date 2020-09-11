if (NOT USE_SEMIHOSTING)
  message("Adding newlib-nano overrides")
  target_sources(${USER_PROJECT_TARGET} PRIVATE syssbrk.c syscalls.c)
endif()
