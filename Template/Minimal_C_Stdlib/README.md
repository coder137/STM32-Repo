# DEBUGGING with openocd

## Starting openocd server

```
openocd.exe -f stm32l4discovery.cfg
```

## Starting GDB

```
arm-none-eabi-gdb.exe <file>.elf
```

### Running Commands

```
target remote localhost:3333

# The monitor command runs openocd commands
monitor reset init

monitor reset halt

monitor flash write_image erase *.elf

monitor resume
```

### Verify Data location

```
monitor mdw 0x20000000 4
```

# CMake Integration


## Starting OpenOCD Server

```
cmake --build build --target openocd_start_server
```

## Starting GDB

```
cmake --build build --target openocd_connect_client
```

> Run your commands similar to the steps given above
