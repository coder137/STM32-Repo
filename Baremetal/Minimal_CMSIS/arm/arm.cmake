# target_sources(${USER_PROJECT_TARGET} PRIVATE "arm/startup_ARMCM4.c")
target_include_directories(
  ${USER_PROJECT_TARGET} PRIVATE "arm/cmsis/core"
                                 # "arm/include"
)
