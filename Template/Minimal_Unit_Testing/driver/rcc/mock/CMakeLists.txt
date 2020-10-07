add_library(mock_rcc INTERFACE)
target_include_directories(mock_rcc INTERFACE 
    .
    ${TEST_INCLUDE_DIRECTORIES}
)
target_link_libraries(mock_rcc INTERFACE fff)
