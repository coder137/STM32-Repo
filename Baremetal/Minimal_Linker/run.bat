@echo off

REM -fno-rtti

REM echo "Creating main"
REM arm-none-eabi-gcc -o main.o -c -fmessage-length=0 -O0 -mcpu=cortex-m4 -mthumb -Wall -g -pipe main.c

echo "Creating main"
arm-none-eabi-gcc -o main.o -c -fmessage-length=0 -ffunction-sections -fdata-sections -O0 -mcpu=cortex-m4 -mthumb -Wall -g -pipe main.c

echo "Linking"
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Tlayout_stm32l4.ld -Wl,--gc-sections -Wl,-Map,main.map -nostartfiles main.o -o main.elf

echo "objcopy lst"
arm-none-eabi-objdump --source --all-headers --demangle --line-numbers --wide main.elf > main.lst

echo "size: format sysv"
arm-none-eabi-size --format=sysv main.elf

echo "size: format berkeley"
arm-none-eabi-size --format=berkeley main.elf

echo "nm for main.elf"
arm-none-eabi-nm main.elf -n -A -l -S

echo "Object Dump for -h main.elf"
arm-none-eabi-objdump -t -h main.elf
