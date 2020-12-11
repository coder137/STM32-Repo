# Comparison

[build] Memory region         Used Size  Region Size  %age Used
[build]            FLASH:       16748 B         1 MB      1.60%
[build]              RAM:       41596 B        96 KB     42.31%
[build] [20/20  95% :: 0.673] cmd.exe /C "cd /D D:\Repositories\STM32-Repo\Template\Minimal_Libraries\build && arm-none-eabi-size --format=berkeley D:/Repositories/STM32-Repo/Template/Minimal_Libraries/build/Minimal_Libraries.elf"
[build]    text	   data	    bss	    dec	    hex	filename
[build]   16725	    135	  41484	  58344	   e3e8	D:/Repositories/STM32-Repo/Template/Minimal_Libraries/build/Minimal_Libraries.elf
[build] Build finished with exit code 0

# After adding cmsis and device static libraries

[build] Memory region         Used Size  Region Size  %age Used
[build]            FLASH:       16764 B         1 MB      1.60%
[build]              RAM:       41596 B        96 KB     42.31%
[build] [20/20  95% :: 0.819] cmd.exe /C "cd /D D:\Repositories\STM32-Repo\Template\Minimal_Libraries\build && arm-none-eabi-size --format=berkeley D:/Repositories/STM32-Repo/Template/Minimal_Libraries/build/Minimal_Libraries.elf"
[build]    text	   data	    bss	    dec	    hex	filename
[build]   16741	    135	  41484	  58360	   e3f8


16 bytes difference

# 

[build] Memory region         Used Size  Region Size  %age Used
[build]            FLASH:       16792 B         1 MB      1.60%
[build]              RAM:       41596 B        96 KB     42.31%
[build] [21/21  95% :: 0.777] cmd.exe /C "cd /D D:\Repositories\STM32-Repo\Template\Minimal_Libraries\build && arm-none-eabi-size --format=berkeley D:/Repositories/STM32-Repo/Template/Minimal_Libraries/build/Minimal_Libraries.elf"
[build]    text	   data	    bss	    dec	    hex	filename
[build]   16769	    131	  41488	  58388	   e414	D:/Repositories/STM32-Repo/Template/Minimal_Libraries/build/Minimal_Libraries.elf
[build] Build finished with exit code 0

28 bytes difference
