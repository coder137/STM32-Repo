- [Interrupt_Vector_Table](#interrupt_vector_table)
- [Explanation](#explanation)
- [What is the *weak* attribute](#what-is-the-weak-attribute)
- [What happens during Reset](#what-happens-during-reset)
- [CMSIS BUGS](#cmsis-bugs)
  - [CMSIS Multi-Segment Issue](#cmsis-multi-segment-issue)

# Interrupt_Vector_Table

How do interrupts run?

- Take a look at `Template/Minimal_Interrupt/l0_lowlevel/device/startup_stm32l4xx.c` OR [Github Link](https://github.com/coder137/STM32-Repo/blob/e9861fb94b6ff8ca84bb3ba4b9617d6c663f76d0/Template/Minimal_Interrupt/l0_lowlevel/device/startup_stm32l4xx.c)


Read this Article by [Interrupt.Memfault](https://interrupt.memfault.com/blog/arm-cortex-m-exceptions-and-nvic)

# Explanation

- The `__VECTOR_TABLE` is loaded at the FLASH intial address (Look at the **Linker Script** for more details)
- On device boot up, the Initial Stack pointer is loaded first
- The `Reset_Handler` runs next
  - `Reset_Handler` by itself is an interrupt which never exits
- All the other functions are interrupts with lower priority
- Priority starts from `-16` till `(255 - 16)`;
- **Negative Priority** is just a convention used to indicate that these functios cannot be Masked i.e they are always active and hardware activated
- **Positive Priority Interrupts** can be masked and also can have custom functions defined by the user.

# What is the *weak* attribute

This attribute in laymans terms is a stub function.

- Weak Attributed functions usually just have a default stub written which can be easily overrided by the user
- To override the default stub the user code should have the same function prototype.

# What happens during Reset

Use the `startup_stm32l4xx.c` as reference (Linked above)

- The interrupt `Reset_Handler` is called after the initial stack pointer is loaded
  - See the **Interrupt Vector Table** pointer locations
- Runs the STM32 specific `System_Init`
  - TODO, This hasnt been researched as of now. (Defined by STM32)
- Runs the CMSIS `__PROGRAM_STARTUP` function
  - Present inside the `cmsis_gcc.h` file
  - Internally calls the `__cmsis_start` function
- Inside `_cmsis_start`
  - Copies the FLASH data section to RAM
  - Initializes the BSS section to 0
  - Calls the externed function `_start` which needs to be user defined
- Look at the `entry_point.c` file for the user defined `_start` function.
- Usage of `_start` function
  - Common Peripheral Initialization (UART driver)
  - Standard Library Initialization (printf usage)
  - Run your `int main` where the user code resides

# CMSIS BUGS

## [CMSIS Multi-Segment Issue](https://github.com/ARM-software/CMSIS_5/issues/632)

- Update the linker script and add your `__bss_start` and size to initialize `.zero.table` section data to 0
  - This NEEDS to be added otherwise our static variables will not have a clear initial value (i.e 0)
- We also need to convert divide the `.zero.table` size and `.copy.table` size by 4
  - This is done to convert it from byte sized to word sized as per ARM standard
