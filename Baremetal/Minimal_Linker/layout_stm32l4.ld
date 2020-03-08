/**
 * References:
 * - https://sourceware.org/binutils/docs/ld/Basic-Script-Concepts.html#Basic-Script-Concepts
 * - https://arobenko.gitbooks.io/bare_metal_cpp/content/
 * 
 * - http://tty.uchuujin.de/2016/02/stm32-from-scratch-bare-minimals/
 * - http://tty.uchuujin.de/2016/03/stm32-from-scratch-enableing-c-c++-features/
 */

MEMORY
{
  flash (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
  ram (rwx)   : ORIGIN = 0x20000000, LENGTH = 96K
  ram2 (rwx)   : ORIGIN = 0x10000000, LENGTH = 32K
}

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
