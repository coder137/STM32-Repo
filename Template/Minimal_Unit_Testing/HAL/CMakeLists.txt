set(output_gpio_files output_gpio/output_gpio.c)
set(input_gpio_files input_gpio/input_gpio.c)

target_sources(${USER_PROJECT_TARGET} PRIVATE 
    ${output_gpio_files}
    ${input_gpio_files}
)
target_include_directories(${USER_PROJECT_TARGET} PRIVATE .)
