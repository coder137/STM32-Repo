add_subdirectory(arm)
add_subdirectory(device)

target_sources(${USER_PROJECT_TARGET} PRIVATE "entry_point.c")
