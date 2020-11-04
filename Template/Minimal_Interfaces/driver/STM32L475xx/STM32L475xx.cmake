# Add all STM32L475xx hardware specific driver sources here
ess_data(driver_sources
    exti/exti.c
    rcc/rcc.c
    gpio/gpio.c
    uart/uart.c
    uart/uart_interrupt.c
)
