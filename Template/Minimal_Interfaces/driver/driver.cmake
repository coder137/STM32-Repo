set(driver_sources "")
set(driver_dirs "")

ess_data(driver_sources
    exti/exti.c
    gpio/gpio.c
    rcc/rcc.c
    uart/uart.c
    uart/uart_interrupt.c
)

ess_data(driver_dirs 
    .
)
