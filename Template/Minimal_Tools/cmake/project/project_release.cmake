set(USER_PROJECT_TARGET "${PROJECT_NAME}.elf")
add_executable(${USER_PROJECT_TARGET}
    ${USER_SOURCES}
)

target_include_directories(${USER_PROJECT_TARGET} PRIVATE
    ${USER_INCLUDE_DIRS}
)

target_compile_options(${USER_PROJECT_TARGET} PRIVATE 
    ${COMMON_C_FLAGS}
    ${USER_C_FLAGS}
)

target_link_options(${USER_PROJECT_TARGET} PRIVATE 
    ${COMMON_C_FLAGS}
    ${USER_LINK_FLAGS}
)
