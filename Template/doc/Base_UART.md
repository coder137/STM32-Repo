- [Base_UART](#base_uart)
- [Registers](#registers)
- [Functional Steps](#functional-steps)
  - [Initialize](#initialize)
  - [How does UART Poll Transmit Work](#how-does-uart-poll-transmit-work)
  - [UART Poll Transmit for 1 Byte](#uart-poll-transmit-for-1-byte)
  - [UART Poll transmit for Multiple Bytes](#uart-poll-transmit-for-multiple-bytes)
  - [UART Poll Receive](#uart-poll-receive)

# Base_UART

Writing the Basic UART driver for STM32 Controller

# Registers

- CR1
- CR2
- CR3

- BRR
- ISR

- TDR
- RDR

# Functional Steps

## Initialize

- Disable the UART
- Set the Word Length
  - Choose from Bit Length 7, 8, 9
- Set your Baud Rate
  - Use the `SystemCoreClock` to calculate the `USARTDIV` value
- Set your Stop Bit
  - 1, 2, 0.5, 1.5
  - Since this was for a UART driver we should only select between 1, 2
- Enable the UART
- Set the MODE
  - Activate Transmit and Receive Flags as required


## How does UART Poll Transmit Work

- We first check to see if `ISR_TXE` is 1 or no.
- This signifies if we can push data to the `TDR` register or no
- Once we push data inside the `TDR` Register `ISR_TXE` and `ISR_TC` go low
- `ISR_TXE` becomes 1 once TDR register is cleared
- After 1 cycle of `ISR_TXE` becoming 1, if data is not filled into `TDR`, `ISR_TC` sets to 1
- Using this behaviour we can transmit for Multiple Bytes

## UART Poll Transmit for 1 Byte

- Wait for `ISR_TXE` to become 1
- Load data to the `TDR` register
- Wait for `ISR_TC` to become 1


## UART Poll transmit for Multiple Bytes

- Wait for `ISR_TXE` to become 1
- Load data to the `TDR` register
- While we have data
  - We wait for `ISR_TXE` to become 1
  - We load data to the `TDR` register
- Once we run out of data
  - Wait for `ISR_TC` to become 1

> NOTE, Read the **How does this work?** section above to understand the behaviour

## UART Poll Receive

- We wait for `ISR_RXNE` bit to be set.
  - This bit getting set means that data has been received by the buffer
- We read data from the `RDR` register
- Once we read data from the `RDR` register, the `ISR_RXNE` bit automatically sets itself to 0
