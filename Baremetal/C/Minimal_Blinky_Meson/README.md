- [Pre-Requisites to using Meson](#pre-requisites-to-using-meson)
- [Meson for Cross Compilation](#meson-for-cross-compilation)
  - [Building for Cross Compilation](#building-for-cross-compilation)
  - [Running for Cross Compilation](#running-for-cross-compilation)
  - [Finding your device](#finding-your-device)
  - [Flashing your device](#flashing-your-device)
    - [Configure your Software Number](#configure-your-software-number)
    - [Flashing](#flashing)

# Pre-Requisites to using Meson

- `arm-none-eabi-` toolchain
- Ninja
- STM32_Programmer_CLI tool

# Meson for Cross Compilation

## Building for Cross Compilation

```
meson build_directory --cross-file cross_toolchain.txt
```

## Running for Cross Compilation

```
meson compile -C build_directory
```

This will do the following

- Compile your **elf** executable
- Convert the **elf** to
  - **bin**
  - **hex**
  - **lst**
- Prints the size of the **elf** executable

## Finding your device

```
ninja find_devices -C build_directory
```

- Finds the devices connected to your PC
- Prints out the Hardware and Software Number of your STLINK

## Flashing your device

### Configure your Software Number

**Read the configuration options for the project**

```
meson configure build_directory
```

**Update the configuration option for the project**

```
meson configure -D stlink_sn=<stlink_sn>
```

- You can find this value by **Finding your device** first
- You can verify the value has been added to your project by reading the configurations again

### Flashing

```
ninja flash -C build_directory
```

- This will make sure your binary file is upto date
- Flash your binary file
