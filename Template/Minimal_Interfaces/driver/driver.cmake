set(driver_sources "")
set(driver_dirs "")

ess_data(driver_sources
    ${CONTROLLER_NAME}/gpio/gpio.c
    uart/uart.c
    uart/uart_interrupt.c
)

ess_include(${CONTROLLER_NAME})

ess_data(driver_dirs 
    .
    ${CONTROLLER_NAME}
)
