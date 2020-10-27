if (NOT USE_SEMIHOSTING)
  message("Adding newlib-nano overrides")
  ess_data(l0_lowlevel_sources 
      syssbrk.c
      syscalls.c
  )
endif()
