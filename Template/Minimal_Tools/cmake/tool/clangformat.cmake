if(NOT CLANGFORMAT_REQUIRED)
    message("TOOL: CLANGFORMAT NOT required")
    return()
endif()

message("TOOL: CLANGFORMAT required")
find_program(clangformat_program
    "clang-format"
    REQUIRED
)

set(USER_CLANG_INC_FILES "")
foreach(dir ${USER_CLANG_INC_DIRS})
    file(GLOB temp CONFIGURE_DEPENDS
        "${dir}/*.h"
    )
    list(APPEND USER_CLANG_INC_FILES ${temp})
endforeach()

set(USER_CLANG_FORMAT_FILES 
    ${USER_CLANG_INC_FILES}
    ${USER_CLANG_SOURCES}
)

# clang-format
add_custom_target(
    clang-format ALL
    COMMAND ${clangformat_program} -i ${USER_CLANG_FORMAT_FILES} --style=file
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "TOOL: CLANGFORMAT running"
    VERBATIM USES_TERMINAL
)
