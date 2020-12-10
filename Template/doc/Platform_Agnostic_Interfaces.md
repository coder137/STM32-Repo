- [Platform Agnostic Interfaces](#platform-agnostic-interfaces)
- [Software layered structure](#software-layered-structure)
  - [L0 System Initialization](#l0-system-initialization)
  - [L1 Third Party System Specific](#l1-third-party-system-specific)
  - [L2 Utils](#l2-utils)
  - [L2 Driver](#l2-driver)
  - [L2 Peripheral](#l2-peripheral)
  - [L3 Third Party functional](#l3-third-party-functional)
  - [L4 User Modules](#l4-user-modules)
  - [L5 Layer Application](#l5-layer-application)
- [Adding a new platform](#adding-a-new-platform)
  - [L0 System Initialization](#l0-system-initialization-1)
    - [CMSIS](#cmsis)
    - [Device Layer](#device-layer)
  - [L1 Third Party System Specific](#l1-third-party-system-specific-1)
  - [L2 Utils](#l2-utils-1)
  - [L2 Driver](#l2-driver-1)
  - [L2 Peripheral](#l2-peripheral-1)
  - [L3 Third Party functional](#l3-third-party-functional-1)
  - [L4 User Modules](#l4-user-modules-1)
  - [L5 Layer Application](#l5-layer-application-1)

# Platform Agnostic Interfaces

- Creating commonly used interfaces that can be re-used amongst different hardware having the same architecture.
- Creating a layered approach to project creation

# Software layered structure

## L0 System Initialization

- CMSIS
- Device Layer
  - peripheral file
  - system initialization file
  - startup file
  - entry point

## L1 Third Party System Specific

- FreeRTOS
  - Other RTOS's that need System specific configurations

## L2 Utils

- Common Utils to be used from L2 layer and above

## L2 Driver

- Common Drivers and implementations
- GPIO
- UART

## L2 Peripheral

- Hardware perpheral startup
  - `extern peripheral__initialize(void)`
- Newlib system calls

## L3 Third Party functional

- FatFS
- Sensors

## L4 User Modules

- User created modules for structure

## L5 Layer Application

- Contains `main.c` with usages from lower layers

# Adding a new platform

## L0 System Initialization

### CMSIS

**Usually this does not change!**

- Here we have all of our header file that are architecture specific i.e
- `core_cm4`, `core_cm3` etc
- Different architectures will have their header files here
- Can be adding from the CMSIS github repository directory and updated periodically as needed

### Device Layer

**This is the layer that requires most work**

- peripheral file
  - This is usually vendor specific
  - See this [Keil Link of supported chips](https://www.keil.com/dd/)
- system initialization file
  - Initialize clock, pll
- startup file
  - Contains the Interrupt Vector Table
- entry point
  - Contains extern functions to
  - Semihosting
  - `peripheral__init`
  - `main`

## L1 Third Party System Specific

**RTOS specific changes required**

- FreeRTOS
  - Mainly requires an update to the `FreeRTOSConfig.h` file
  - Also update the heap usage i.e `heap_3` or `heap_4`

## L2 Utils

**No changes required**

- Common Utils to be used from L2 layer and above

## L2 Driver

**Add your vendor specific peripheral code**

- Common Drivers and implementations
- GPIO
- UART

## L2 Peripheral

**Initalizing the System UART and Newlib syscalls**

- Hardware perpheral startup
  - `extern peripheral__initialize(void)`
- Newlib system calls

## L3 Third Party functional

**Mostly does not require changes since it uses L2 Driver code**

- FatFS
- Sensors

## L4 User Modules

**Does not require change**

- User created modules for structure

## L5 Layer Application

**Each processor requires it own _main.c file. In our project we use ${CONTROLLER_NAME}_main.c in cmake**

- Contains `main.c` with usages from lower layers
