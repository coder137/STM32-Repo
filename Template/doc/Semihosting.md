- [Semihosting](#semihosting)
- [Pre-Requisites](#pre-requisites)
- [Important](#important)
- [Code Changes](#code-changes)
- [Build Integration](#build-integration)
  - [CMake Integration](#cmake-integration)
- [Running the executable](#running-the-executable)
  - [CLI](#cli)
  - [VSCode](#vscode)

# Semihosting

Semihosting is equivalent to a UART driver

However, your output is forwarded to the debug terminal (OpenOCD Server) instead of your COM port.

# Pre-Requisites

- OpenOCD

# Important

- Semihosting is a replacement for your C Standard library `<stdio>` based `printf` and `scanf` functions
  - For compilation avoid adding your user defined `syscalls`
  - Read [Newlib_syscalls.md](Newlib_syscalls.md)
- There are specific BUILD changes when using semihosting
  - Read the **build-integration** section
- Semihosting based `scanf` is thread-blocking

# Code Changes

- In your `entry_point.c` (before `main()` runs) add the following function
```cpp
extern void initialise_monitor_handles(void);

void _start() {
  // .user code

#if SEMIHOSTING
  initialise_monitor_handles();
#endif

  main();
}
```

# Build Integration

- Make sure `C Stdlib _syscalls` are NOT added to your compilation.
- Specific Linker Flags needs to be added
```
# newlib-nano library 
-specs=nano.specs

# semihosting library
-specs=rdimon.specs 
```
- Add a preprocessor macro for compile time checking of the above C code
```
-DSEMIHOSTING=1
```

## CMake Integration

```cmake
target_link_options(${USER_PROJECT_TARGET} PRIVATE 
  ${COMMON_C_FLAGS}
  ${USER_SEMIHOSTING_FLAGS}
  ${USER_LINK_FLAGS}
  )
target_compile_definitions(${USER_PROJECT_TARGET} PRIVATE SEMIHOSTING=1)
```

# Running the executable

## CLI

- Connect to the [openocd server](Openocd_gdb_cli.md) via gdb
- Reset init your executable
```
# Important
monitor reset init

# Enable semihosting
monitor arm semihosting enable

# Start your execution from here
```

## VSCode

- Update your configuration file

```json
{
  "configurations": [
    {
      // ...
      "postLaunchCommands": [
        "monitor arm semihosting enable",
      ],
      // ...
    }
  ]
}

```

- Start your debug instance
- Once you start your debugging, you should see logs in the debug terminal
