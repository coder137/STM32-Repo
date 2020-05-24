/**
 * References:
 * - https://sourceware.org/binutils/docs/ld/Basic-Script-Concepts.html#Basic-Script-Concepts
 * - https://arobenko.gitbooks.io/bare_metal_cpp/content/
 * 
 * - http://tty.uchuujin.de/2016/02/stm32-from-scratch-bare-minimals/
 * - http://tty.uchuujin.de/2016/03/stm32-from-scratch-enableing-c-c++-features/
 * 
 * LOCATION COUNTER is always VMA
 */

MEMORY
{
  flash (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
  ram (rwx)   : ORIGIN = 0x20000000, LENGTH = 96K
  ram2 (rwx)   : ORIGIN = 0x10000000, LENGTH = 32K
}

/*************************************************
                                                  *
     64K RAM +-----Stack Top   +-----+ 32K RAM    *
             |    ||           |     |            *
             |    \/           |     |            *
             |     |           |     |            *
             |     |           |     |            *
             |    /\ heap      |    /\            *
             |    || overflow  |    ||            *
             .-----.           +-----Heap Start   *
             |bbbbb|           0x2000000          *
             |bbbbb| *bss                         *
             .-----.                              *
             |ddddd|                              *
             |ddddd| *data                        *
             .-----.                              *
            0x1000000                             *
                                                  *
**************************************************
*/

__base_ram = ORIGIN(ram);
__end_ram = __base_ram + LENGTH(ram);
__istack = __end_ram;

SECTIONS
{
  .text : {
    *(.isr_vector*)
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
    _etext = .;
  } > flash

  .data : {
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > ram AT > flash /* ram is vma, flash is lma */

  .bss : {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > ram
}
