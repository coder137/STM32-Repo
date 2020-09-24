- [OpenOCD with GDB on the Command Line](#openocd-with-gdb-on-the-command-line)
- [Pre-Requisites](#pre-requisites)
- [Start OpenOCD (Server)](#start-openocd-server)
- [Start GDB (Client)](#start-gdb-client)
  - [Connect to Server](#connect-to-server)
  - [Run commands](#run-commands)
    - [Target State Handling](#target-state-handling)
    - [Memory Access Commands](#memory-access-commands)
    - [Breakpoint and Watchpoint Commands](#breakpoint-and-watchpoint-commands)

# OpenOCD with GDB on the Command Line

- [OpenOCD Documentation](http://openocd.org/doc-release/html/index.html)
  - See General Commands
- [Flashing via OpenOCD](http://openocd.org/doc/html/Flash-Programming.html)

# Pre-Requisites

- [GCC ARM](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) should be installed
  - `arm-none-eabi-gdb --version` to check if gdb exists
- [Install openOCD](http://openocd.org/getting-openocd/) using the pre-configured binaries
- Set your openOCD to PATH
  - Ex: `D:\Software\openocd\bin`
- Check the board script files **.cfg**
  - Ex: `D:\Software\openocd\scripts\board`
  - Copy your required board script file to your project
  - For example: `stm32l4discovery.cfg`
- Flash your project (.bin) using the `STM32_Programmer_CLI` tool
  - Run `cmake --build build --target flash` from your project

# Start OpenOCD (Server)

```
# Check version
openocd --version

# Run your openOCD server
openocd -f stm32l4discovery.cfg
```

- Check the port that openOCD is connected to
  - Ex: `Listening on port **xyz**`

# Start GDB (Client)

```
# Start gdb with an executable file
arm-none-eabi-gdb [your_binary.elf]

# You should get a gdb shell
(gdb)
```

## Connect to Server

**Refer to the openOCD Documentation (link provided above)**

```
# Port most commonly used is 3333
target remote localhost:<port>
```

## Run commands

Use `monitor` to talk to the openOCD instance

### [Target State Handling](http://openocd.org/doc-release/html/General-Commands.html#Target-State-handling)

```
# Reset and initialize the starting point
# This is a very important command
monitor reset init

# Write your program
monitor flash write_image erase image.elf

# Reset and halt
monitor reset halt

# Resume
monitor resume
```

### [Memory Access Commands](http://openocd.org/doc-release/html/General-Commands.html#Memory-access-commands)

```
# Check the .map file for the memory_address
monitor mdw <your_memory_address> <words_to_read>
```

### [Breakpoint and Watchpoint Commands](http://openocd.org/doc-release/html/General-Commands.html#Breakpoint-and-Watchpoint-commands)

```
# Set a breakpoint
# Check the .map file for the memory_address
monitor bp <your_memory_address> <length_bytes> [hw]
```
