- [Base_GPIO](#base_gpio)
- [Registers](#registers)
- [Functional Steps](#functional-steps)
  - [Initialize RCC](#initialize-rcc)
  - [Initialize GPIO](#initialize-gpio)
- [GPIO Specific](#gpio-specific)
  - [GPIO Output Mode](#gpio-output-mode)
  - [GPIO Input Mode](#gpio-input-mode)
- [TIPS](#tips)

# Base_GPIO

Writing a Basic GPIO driver for the STM32 Controller

# Registers

- MODER
- OTYPER
- OSPEEDR
- PUPDR
- AFR

- BSRR
- BRR


# Functional Steps

## Initialize RCC

- Activate the `RCC_AHB` (Advanced High Performance bus) for particular GPIO pins
  - By default the Peripherals are in the power off state
  - This is done for power conservation

## Initialize GPIO

- MODER is used to select the various Modes of the Device
  - Input, Output, Alternate Function or Analog
- OTYPER is used to select the Push Pull or Open Drain type of pin configuration
  - Open Drain can only sink current
  - Push pull can sink current as well as drive (output current)
- OSPEEDR is used to set the speed of GPIO change from high to low or low to high
- PUPDR is used to set internal pullup, pulldown OR not pull.
  - Pull up has resistor connected to Voltage High (3.3V) i.e Not Connected state would output a 3.3V
  - Pull down has resistor connected to Voltage Low (0V) i.e Not Connected state would output a 0V
  - No Pull leaves the pin is a potential HiZ state i.e Infinite resistance if there is no external Pullup or Pulldown resistor
- AFR is used to set the Alternate Function for the Pin
  - These details are mentioned in the **Hardware Manual** since they are hardware specific (Each MCU will have their own configurations)
  - AFR is used for UART in this case

# GPIO Specific

## GPIO Output Mode

- MODER needs to be in the Output Mode
- OTYPER needs to be in Push Pull mode
- OSPEEDR can be any mode however LOW SPEED mode saves power
- PUPDR is recommended to be NO Pull since it drives the output
- AFR is default 0 (which usually defaults to the GPIO pin)

- BSRR and BRR are atomic operations i.e they happen in once cycle
- For setting the Pin HIGH -> BSRR 1 the appropriate pin location
- For setting the Pin LOW -> BRR 1 the appropriate pin location

## GPIO Input Mode

- MODER needs to be in the INPUT Mode
- OTYPER needs to be in the Push Pull mode
- OSPEEDR is defaulted to LOW SPEED mode (save power)
- PUPDR should be either PULL UP or PULL DOWN depending on the external resistor attached
- AFR is default 0 (which usually defaults to the GPIO pin)

# TIPS

- Always clear the GPIO PORT at the pin number that you are configuring. DO NOT depend on RESET values
  - In my case I got stuck when configuring the MODER since its reset value is 1 instead of 0
- Writing to Variable is faster than writing to Registers
  - Always write/update your variable
  - Write to each Register only once
