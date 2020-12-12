file(GLOB_RECURSE module_files CONFIGURE_DEPENDS *.c)
message("module_files: ${module_files}")

target_sources(${USER_PROJECT_TARGET} PRIVATE ${module_files})
target_include_directories(${USER_PROJECT_TARGET} PRIVATE .)
