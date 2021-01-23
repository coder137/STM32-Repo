# Add all STM32L475xx hardware specific driver sources here
ess_data(l2_driver_sources
    exti/exti.c
    rcc/rcc.c
    gpio/gpio.c
    uart/uart.c
    uart/uart_interrupt.c
)
ess_data(l2_driver_dirs
    exti
    gpio
    rcc
    uart
)
