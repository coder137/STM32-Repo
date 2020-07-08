- [Newlib Nano](#newlib-nano)
  - [Flags used to link newlib](#flags-used-to-link-newlib)
    - [C Flags](#c-flags)
    - [Link Flags](#link-flags)
- [Implement Stubs](#implement-stubs)
  - [Adding float support to printf and scanf](#adding-float-support-to-printf-and-scanf)

# Newlib Nano

Getting started with Newlib Nano

- [Newlib Documentation](https://sourceware.org/newlib/)

## Flags used to link newlib

### C Flags

- `-nostdlib`
  - Do not use standard system startup files on startup
  - Compiler might require the standard `mem{}` operations
  - These can be supplied by manually linking the `-lc` or `-lgcc` flag

### Link Flags

- `-nostartfile`
  - Does not use the standard starup file when linking
  - You can define your own entry point
- `-lc`
  - C Library
- `-lm`
  - Math Libary
- `nano.specs`
  - Links the **Newlib Nano** library instead of the complete Newlib library
- `nosys.specs`
  - Removes semihosting completely

# Implement Stubs

- `malloc` based functions
  - Allocating memory on the heap
- `printf` and `scanf` based functions
  - Functional logging statements used by C Standard Library

## Adding float support to printf and scanf

Add `-u _printf_float` and `-u _scanf_float` to **linker options**

- Adding these options could potentially add almost 10-20K worth of code space
