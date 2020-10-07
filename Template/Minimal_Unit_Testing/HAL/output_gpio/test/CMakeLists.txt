add_executable(output_gpio_test output_gpio_test.c)

target_sources(output_gpio_test PRIVATE 
    ../output_gpio.c
)

# TODO, Make this better
target_include_directories(output_gpio_test PRIVATE 
    ../
)
target_compile_definitions(output_gpio_test PRIVATE "STM32L475xx")

target_link_libraries(output_gpio_test PRIVATE mock_rcc mock_gpio)
target_link_libraries(output_gpio_test PRIVATE unity fff)
add_test(NAME output_gpio_test COMMAND output_gpio_test)
