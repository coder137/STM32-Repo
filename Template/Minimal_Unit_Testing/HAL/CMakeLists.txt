file(GLOB_RECURSE hal_files CONFIGURE_DEPENDS *.c)

target_sources(${USER_PROJECT_TARGET} PRIVATE ${hal_files})
target_include_directories(${USER_PROJECT_TARGET} PRIVATE .)
