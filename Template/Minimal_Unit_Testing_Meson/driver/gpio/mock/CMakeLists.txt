add_library(mock_gpio INTERFACE)
target_include_directories(mock_gpio INTERFACE 
    .
    ${TEST_INCLUDE_DIRECTORIES}
)

target_link_libraries(mock_gpio INTERFACE fff)
