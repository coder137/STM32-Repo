- [Interrupt_UART](#interrupt_uart)
- [Explanation](#explanation)
  - [Pre-requisite](#pre-requisite)
  - [Initialize](#initialize)
- [UART Interrupt Specific](#uart-interrupt-specific)
  - [Interrupt Receive](#interrupt-receive)
  - [Interrupt Transmit](#interrupt-transmit)
    - [Trasmit Enable](#trasmit-enable)
    - [Transmit Complete](#transmit-complete)
  - [[TODO ]Interrupt Errors](#todo-interrupt-errors)
- [API Design](#api-design)
  - [READ API](#read-api)
  - [WRITE API](#write-api)
  - [Transmit STATES](#transmit-states)
    - [Transmit Enable **DISABLED** and Transmit Complete **DISABLED**](#transmit-enable-disabled-and-transmit-complete-disabled)
    - [Transmit Enable **DISABLED** and Transmit Complete **ENABLED**](#transmit-enable-disabled-and-transmit-complete-enabled)
    - [Transmit Enable **ENABLED** and Transmit Complete **DISABLED**](#transmit-enable-enabled-and-transmit-complete-disabled)
    - [Transmit Enable **ENABLED** and Transmit Complete **ENABLED**](#transmit-enable-enabled-and-transmit-complete-enabled)

# Interrupt_UART

[Base_UART](Base_UART.md) used polling to transmit and Receive data.

Configure UART to use Interrupts to Transmit and Receive data

# Explanation

## Pre-requisite

- Initialize the UART according to the [Base_UART document](Base_UART.md)

## Initialize

- RCC is initialized when initializing the UART for polling
- We only need to activate specific interrupts in the `USART_CR1` register
- After activating our specific interrupts we call `NVIC_EnableIRQ` function


# UART Interrupt Specific

- We have used `USART1` in our project. 
- The `USART1_Handler` Interrupt function runs when any `USART1` specific interrupt is received
- The specific interrupt bits need to be set in the USART **Control Registers**

## Interrupt Receive

- According to our **Base_UART** document when data is received the `ISR_RXNE` bit is set
- We enable this receive interrupt by setting the `USART_CR1_RXINE` bit
- When an interrupt is received we check
  - if `ISR_RXNE` bit is set
  - if `CR1_RXINE` bit is set
- We then read from the `RDR` register
  - Reading from the `RDR` register clears the `ISR_RXNE` interrupt flag

## Interrupt Transmit

- According to our **BASE_UART** document when we want to send data (`TDR` register) the `ISR_TXE` bit should be high. 
- `ISR_TC` bit high means that Transmission is complete and a new transmission can begin.
- Interrupts can be set for both the `ISR_TXE` and `ISR_TC` which are triggered when they are logic 1
  - `CR1_TXEIE` for `ISR_TXE`
  - `CR1_TCIE` for `ISR_TC`

### Trasmit Enable

- When `CR1_TXEIE` is set, interrupt is generated when `ISR_TXE` is logic 1
- We write to the `TDR` register 
- `ISR_TXE` becomes logic 0
- Once `TDR` register starts transmitting and is cleared, `ISR_TXE` is set and another interrupt is generated
- When the interrupt is received we check
  - if `ISR_TXE` is set
  - if `CR1_TXEIE` is set
- IF we have data to write
  - Write data to the `TDR` register
- ELSE
  - We disable this interrupt by clearing the `CR1_TXEIE` bit 


### Transmit Complete

- When `CR1_TCIE` is set, interrupt is generated with `ISR_TC` is logic 1
- As per the usage of `ISR_TC` it is only set ONE CYCLE after `ISR_TXE` is set (from 0 - 1)
- When the interrupt is received we check
  - if `ISR_TC` is set
  - if `CR1_TCIE` is set
- Once we get this flag we can perform Transmit Complete operations
  - Reset the QUEUE that writes to the `TDR` register
  - Run user defined code

## [TODO ]Interrupt Errors

> There are a lot of errors that can take place when Receiving and Transmitting data

> For now this has been ignored to keep the UART State Machine simple

# API Design

## READ API

- Inside the `ISR_RXNE` interrupt we read the data
- Add data to a `queue`
  - Ring Buffer
  - RTOS Queue
- When the `uart_interrupt__read` API is called
  - Read once character from the `queue` for user program

## WRITE API

- Inside the `ISR_TXE` interrupt we get data from the **Transmit Queue**
- **Transmit Queue**
  - Ring Buffer
  - RTOS Queue
- When the `uart_interrupt__write` API is called
  - CHECK if `CR1_TXEIE` and `CR1_TCIE` is enabled

## Transmit STATES

### Transmit Enable **DISABLED** and Transmit Complete **DISABLED**

`(CR1_TXEIE == 0) && (CR1_TCIE == 0)`

- Fill the QUEUE
- Activate both `CR1_TXEIE` and `CR1_TCIE` interrupts

### Transmit Enable **DISABLED** and Transmit Complete **ENABLED**

`(CR1_TXEIE == 0) && (CR1_TCIE == 1)`

- CR1_TXEIE has just been disabled and in the next cycle CR1_TCIE will be disabled
- Reset the QUEUE (if needed)
- Fill the Transmit Queue wrt data received
- Activate `CR1_TXEIE` interrupt

### Transmit Enable **ENABLED** and Transmit Complete **DISABLED**

`(CR1_TXEIE == 1) && (CR1_TCIE == 0)`

- As per code design this state is currently not possible. 
- Since we are activating both `TXEIE` and `TCIE`, `TXEIE` will always be disabled first

### Transmit Enable **ENABLED** and Transmit Complete **ENABLED**

`(CR1_TXEIE == 1) && (CR1_TCIE == 1)`

- Both the Interrupts are active, which means that there is data in the QUEUE to be transmitted
- We append our new data to the QUEUE
