add_subdirectory(arm)
add_subdirectory(device)
add_subdirectory(newlib)

target_sources(${USER_PROJECT_TARGET} PRIVATE "entry_point.c")
