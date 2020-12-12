- [Integrate Threadx](#integrate-threadx)
  - [Important Links](#important-links)
  - [Linker script/Symbol considerations](#linker-scriptsymbol-considerations)
- [Steps](#steps)
  - [Threadx library](#threadx-library)
  - [CMake considerations](#cmake-considerations)
  - [tx_initialize_low_level Assembly](#tx_initialize_low_level-assembly)
  - [tx_initialize_low_level C](#tx_initialize_low_level-c)
    - [Disable Interrupts](#disable-interrupts)
    - [Set base of the Memory to unused region](#set-base-of-the-memory-to-unused-region)
    - [Setup Vector Table Offset](#setup-vector-table-offset)
    - [Set Stack Pointer value](#set-stack-pointer-value)
    - [Configure DWT](#configure-dwt)
    - [Configure SysTick](#configure-systick)
    - [Configure Handler priorities](#configure-handler-priorities)

# Integrate Threadx

Integrating the Microsoft Threadx RTOS

## Important Links

- [Azure RTOS Github repository](https://github.com/azure-rtos)
- [Threadx Github repository](https://github.com/azure-rtos/threadx)

Currently using ThreadX 6.0/6.x

## Linker script/Symbol considerations

- Requires a memory free space linker symbol
  - This usually comes after the `__HeapLimit` symbol
  - The free space between `__HeapLimit`and `__StackLimit` in our linker script
  - Make sure there is ample space for the ThreadX RTOS

# Steps

## Threadx library

As per the root level CMakeLists.txt we need to set two variables
- THREADX_ARCH: `cortex_m4`
- THREADX_TOOLCHAIN: `gnu`

We can see `THREADX_ARCH` in the ports folder
After selecting our `THREADX_ARCH` we can choose our toolchain `THREADX_TOOLCHAIN`

## CMake considerations

- The above configurations are added to the `add_library` threadx static library
  - Conversely these files can also be added to the entire .elf created but since we are not modifying any files we can create a library.
- The created library is then consumed by the final executable using the `target_link_libraries` command.
- We also need to add `tx_initialize_low_level.S` assembly file
  - The demo initialize file can be found under `port/THREADX_ARCH/THREADX_TOOLCHAIN/example_build`

## tx_initialize_low_level Assembly

The `tx_initialize_low_level.S` file requires several symbols to function properly

- `__HeapLimit` that signifies the heap end
  - This is needed since we usually have freespace between the `__HeapLimit` and `__StackLimit`
  - See your linker script for more details
- `__Vectors` is the Interrupt Vector Table that contains all the startup requirements
- `SYSTEM_CLOCK` is the constant value of your Microcontroller clock speed (SYSCLK)
  - We can also use the CMSIS `SystemCoreClock` externed variable to find our system clock speed

## tx_initialize_low_level C

### Disable Interrupts

- We can use the CMSIS function `__disable_irq`

### Set base of the Memory to unused region

- `_tx_initialize_unused_memory` is a ThreadX variable that should be set to the start of the unused/free memory location
- It should be aligned to 4 bytes (+4)
- In our case we have free space between `__HeapLimit` and `__StackLimit`
- Hence we can set the address of the above value to `addressof(__HeapLimit) + 4`
- Read the linker script for more details

### Setup Vector Table Offset

- This decides if we want to store the Interrupt Vector Table in Flash or RAM
- Check the **PM0214 Programming Manual -> SCB_VTOR Registers**
- Added to `<root>/doc` folder


### Set Stack Pointer value

- `_tx_thread_system_stack_ptr` is the second ThreadX variable that should be set to the start of the Stack
- The `__StackTop` or `__INITIAL_SP` variable is also used as the first value of our interrupt vector table

### Configure DWT

- Please read **ARMv7-M Architecture Reference Manual: Page C1-737** for more details
- Added to `<root>/doc/ARM` folder

### Configure SysTick

- SysTick needs to be loaded with the appopriate value to generate interrupts are regular intervals
- This is also used for the correct functioning of the ThreadX RTOS.
- Check the **PM0214 Programming Manual -> SysTick Registers**
- Added to `<root>/doc` folder


### Configure Handler priorities

- Check the **PM0214 Programming Manual -> SCB_SHP Registers**
- Added to `<root>/doc` folder
