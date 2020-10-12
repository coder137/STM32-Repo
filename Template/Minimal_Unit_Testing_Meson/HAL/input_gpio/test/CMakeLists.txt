add_executable(input_gpio_test input_gpio_test.c)

target_sources(input_gpio_test PRIVATE 
    ../input_gpio.c
)

# TODO, Make this better
target_compile_definitions(input_gpio_test PRIVATE "STM32L475xx")

target_link_libraries(input_gpio_test PRIVATE mock_rcc mock_gpio)
target_link_libraries(input_gpio_test PRIVATE unity fff)
# These warnings are hit due to the CMSIS files
target_compile_options(input_gpio_test PRIVATE -Wno-int-to-pointer-cast)

add_test(NAME input_gpio_test COMMAND input_gpio_test)
