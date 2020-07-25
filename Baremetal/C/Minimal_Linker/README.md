- [Minimal_Linker](#minimallinker)
- [Pre-requisite understanding](#pre-requisite-understanding)
  - [Other Links](#other-links)
  - [Compilation and Linking with **arm-none-eabi-gcc**](#compilation-and-linking-with-arm-none-eabi-gcc)
    - [Compilation](#compilation)
    - [Linking](#linking)
    - [Other commands](#other-commands)
  - [Writing the Linker Script and using **arm-none-eabi-objdump**](#writing-the-linker-script-and-using-arm-none-eabi-objdump)
  - [Understanding Sections](#understanding-sections)
    - [Created for the **minimal_linker** example project](#created-for-the-minimallinker-example-project)
    - [What it really is?](#what-it-really-is)
    - [Deep diving inside the **main.map** file to understand the sections](#deep-diving-inside-the-mainmap-file-to-understand-the-sections)
  - [Code converted to Code Sections](#code-converted-to-code-sections)
    - [TEXT section](#text-section)
    - [RODATA section](#rodata-section)
    - [BSS section](#bss-section)
    - [COMMON section](#common-section)
    - [DATA section](#data-section)
  - [Other sections](#other-sections)

# Minimal_Linker

AIM: Understand and Writing your own linker scripts

# Pre-requisite understanding

- [Read this amazing tutorial](https://interrupt.memfault.com/blog/how-to-write-linker-scripts-for-firmware)
- [Read the STM32 getting started from scratch guide](http://tty.uchuujin.de/2016/02/stm32-from-scratch-bare-minimals/)
  - [Github Link to the same](https://github.com/textshell/stm32-from-scratch)
- **Code** is converted to **Code Sections** which get placed into **User defined Sections** as written in our `linker script`
- `*` is a wildcard for 0+ characters

## Other Links

- [Sourceware Linker Documentation](https://sourceware.org/binutils/docs/ld/Scripts.html#Scripts)
- [GCC Documentation](https://gcc.gnu.org/onlinedocs/gcc/index.html#SEC_Contents)

## Compilation and Linking with **arm-none-eabi-gcc**

### Compilation

```bash
arm-none-eabi-gcc -o main.o -c -fmessage-length=0 -ffunction-sections -fdata-sections -O0 -mcpu=cortex-m4 -mthumb -Wall -g -pipe main.c
```

- `ffunction-sections` and `fdata-sections` are important to generate the section inside the `main.map` later
- `-O0` and `-g` means no optimization and debug activated
- Investigate the other flags used

### Linking

```bash
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Tlayout_stm32l4.ld -Wl,--gc-sections -Wl,-Map,main.map -nostartfiles main.o -o main.elf
```
- `-Tlayout_stm32l4.ld` specifies our linker script used
- `-Wl,--gc-sections` remove the unused sections from our code
  - Use the `KEEP()` linker command so that the unused code is not garbage collected
- `-Wl,-Map,main.map` created a `.map` file for us (disassembly of section and symbols)
- Investigate the other flags used

### Other commands

Take a look at [the project dump file](dump/compile_dump_scons_a_i2c_build.txt) which contains a generic compilation procedure for a default ARM project

## Writing the Linker Script and using **arm-none-eabi-objdump**

```bash
arm-none-eabi-objdump -t -h main.elf
```

- We use the `-t` flag to generate the **SYMBOL TABLE**
- We use the `-h` flag to generate a more verbose **LINKER TABLE** 
    - Gives us the **section name**, **vma**, **lma**, **size of section** and **alignment of section**

## Understanding Sections

### Created for the **minimal_linker** example project
```
SECTIONS
{
  .text : {
    KEEP(*(.isr_vector*))
    *(.text*)
    *(.rodata*)
  } > flash

  .bss (NOLOAD) : {
    *(.bss*)
    *(COMMON)
  } > ram

  .data : {
    *(.data*)
  } > ram AT > flash /* ram is vma, flash is lma */

  /* Check might not be needed */
  .stack : {
    . = ALIGN(8);
  } > ram
}
```

### What it really is?

```
.your_section_inside_hardware : {
  *(.section_generated_by_code)
  *(.add_more_sections_generated_by_code)
} > memory_location_virtual_memory AT > memory_location_load_memory
```

**NOTE: if `AT > lma` not defined `vma == lma` for that section**

### Deep diving inside the **main.map** file to understand the sections

- Inside the `main.map` file on **line 43** onwards, we can see the section that **WE** defined and the **code generated sections** added to it.
- `.text` section contains all `isr_vector*`, `text*` and `rodata*` instances
- The sections after that have been generated and placed by the compilers (TODO, Learn more about them)
- **Line 70** contains `.bss` section searches for all `bss*` and `COMMON*` fields. However they have been **discarded** due to the `-Wl,--gc-sections` flag during linker process
  - See `run.bat` to understand the command that has been passed
  - See the **Discarded Input Sections** on **Line 8** of `main.map`
- **Line 74** contains `.data` section and searches for all `data*` fields

## Code converted to Code Sections

Check the `main.c` file for comments

### TEXT section

- functions are added here

### RODATA section

**Initialized** AND **Constant**

- Global Initialized constants
- Static Initialized constants

### BSS section

**Static** AND **Uninitialized**

- Static Uninitialized variables
- Static Uninitialized constants

### COMMON section

**Global** AND **Uninitialized**

- Global Uninitialized variables
- Global Uninitialized constants

### DATA section

**Initialized** AND **Non Constant**

- Global initialized variables
- Static initialized variables

## Other sections

- Variables inside function
  - Local variables have no effect -> Added to **text** section
  - Statically defined variables are added to **rodata**, **bss** or **data** appropriately
  - NOTE, Not added to **COMMON** since the variable is not **global**

- Pointer Variables
  - These behave in the exact same way as the types above
  - `static int *static_ptr` -> **BSS**
  - `static const int *const_static_ptr` -> **BSS**
  - `int *global_ptr` -> **COMMON**

- dynamically allocated variables
  - This feature comes into play with the `#include <stdlib>` function
  - TODO, Test `malloc`, `free` for C
  - TODO, Test `new`, `delete` for C++

> There maybe more, add here
