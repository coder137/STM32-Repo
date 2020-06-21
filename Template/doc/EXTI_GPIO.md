- [EXTI_GPIO](#exti_gpio)
- [Explanation](#explanation)
  - [Pre-requisite](#pre-requisite)
  - [Initialize RCC and SYSCFG](#initialize-rcc-and-syscfg)
  - [Initialize EXTI](#initialize-exti)
  - [Initialize NVIC](#initialize-nvic)
- [Using NVIC](#using-nvic)

# EXTI_GPIO

External Interrupts on GPIO

**Read the PM0214 Programming Manual to know more about ARM NVIC**

# Explanation

## Pre-requisite

- Configure your GPIO pin as INPUT ([Read the BASE_GPIO Document](Base_GPIO.md))

## Initialize RCC and SYSCFG

- Enable `SYSCFG` using the `RCC_APB2` Register
- Use the `SYSCFG_EXTICR` Register 

## Initialize EXTI

- Set `EXTI_IMR` (Interrup Mask) Register to map GPIO pin for interrupt
- Reset the `EXTI_EMR` (Event Mask) Register to disable events
- Set the `EXTI_FTSR` and `EXTI_RTST` for Falling and Rising edge configurations

## Initialize NVIC

- NVIC (Nested Vector Interrupt Controller) is used to perform the ARM Interrupt related Tasks
  - In this situation we want to use the `NVIC_EnableIRQ` function
  - The `enum IRQn_Type` is the index position of the Interrupt function on the `Interrupt Vector Table`
  - Example: In this case since we are activating INPUT pin GPIO13 we need to use the `EXTI15_10_Handler` interrupt function
- Inside the `EXTI15_10_Handler` interrupt function
  - Check the `EXTI_PR` register for Pending interrupt
  - Clear the `EXTI_PR` register bit by writing 1 to it 
  - This is done to signify that we have handled the interrupt
  - Write your own user code to handle the interrupt

# Using NVIC

> Research more about NVIC

- In this example only the `NVIC_EnableIRQ` function has been used
- Read more details in the **PM0214 Programming Manual**
