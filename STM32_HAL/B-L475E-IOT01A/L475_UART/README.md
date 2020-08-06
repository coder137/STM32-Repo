# L475_UART

HAL UART Example to

- Write via polling to console
- Read via polling from console
- Read via interrupt from console
- Implement `printf` and `scanf`

# Using newlib `printf` and `scanf` functions

## printf

- For printf we overload the `_write` function
  - We can see a generic implementation of this in `syscalls.c`
  - We can also see the function prototype inside `unistd.h`
  - Implemented via polling by calling the `HAL_UART_Transmit` function

## scanf 

For scanf we overload to the `_read` function
  - We can see a generic implementation of this in `syscalls.c`
  - WE can also see the function prototype inside `unistd.h`
  - Implemented via polling by calling the `HAL_UART_Receive` function
  - TODO, Can do it via the `HAL_UART_Receive_IT` function

# Receive via interrupt

- We can put a buffer and get bulk data via interrupt.
  - However this does not seem to work very well
  - Implementation receives one byte of data and after making a check manually adds that data to a pre-allocated buffer
- TODO, Test receiving a string over a buffer directly

# Transmit via interrupt

- Calls the `HAL_UART_Transmit_IT` function (non-blocking)
- TODO, Use with FreeRTOS and write wrapper to use with mutex

# TODO

- [ ] Implement `scanf` -> `_read` function via the `HAL_UART_Receive_IT` function
- [ ] Test -> Receive a string of data via `HAL_UART_Receive_IT(buf[])`
- [ ] Write your own `printf` function using `va_args` using `HAL_UART_Transmit_IT` function
