- [DEBUGGING with openocd](#debugging-with-openocd)
  - [Starting openocd server](#starting-openocd-server)
  - [Starting GDB](#starting-gdb)
    - [Running Commands](#running-commands)
    - [Verify Data location](#verify-data-location)
- [CMake Integration](#cmake-integration)
  - [Starting OpenOCD Server](#starting-openocd-server-1)
  - [Starting GDB](#starting-gdb-1)
- [VSCode Integration with OpenOCD](#vscode-integration-with-openocd)
- [Unit Testing and Mocking](#unit-testing-and-mocking)
  - [IMPORTANT](#important)
  - [Configuring your Unit-Testing project](#configuring-your-unit-testing-project)
  - [Running the Unit Tests](#running-the-unit-tests)
- [Adding your custom Unit-Test](#adding-your-custom-unit-test)
  - [Add your include dependencies](#add-your-include-dependencies)

# DEBUGGING with openocd

Copy the board configuration file

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

# VSCode Integration with OpenOCD

Install the **Cortex-Debug** plugin

[OpenOCD Specific Configuration](https://github.com/Marus/cortex-debug/wiki/OpenOCD-Specific-Configuration)

# Unit Testing and Mocking

## IMPORTANT

- CMake can only have one compiler stored in its Cache
- When building your ARM project you cannot have your native compiler testing as well


## Configuring your Unit-Testing project

- Delete your build folder
- In the **root level CMakeLists.txt** set your **TESTING** flag to **ON**
- Reconfigure your project using the below command

```cmake
cmake -B build -G Ninja -D CMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE
```

## Running the Unit Tests

- From the build directory
```
ctest -T test --output-on-failure
```

# Adding your custom Unit-Test

> TODO, Update this

## Add your include dependencies 

- As of now CMake does not have the capability to detect the include requirements of the file and make mocks
- The include dependencies need to be manually added
