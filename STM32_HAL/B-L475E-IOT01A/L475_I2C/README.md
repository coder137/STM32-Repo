# L475_I2C

**Copied L475_UART code and renamed it to L475_I2C to reuse the printf and scanf code**

# Notes

- Added -u _printf_float and -u _scanf_float linker flags to the project to print floating numbers
- Tested the I2C HAL Driver by writing getting Temperature data from HTS221
- Document is present in the `doc/components` folder
- Read the Register Map on Page 26 carefully. Some data bits needs to be signed vs unsigned (negative values are permitted i.e MSB is 1)

## HAL I2C

- `HAL_I2C_Mem_Read` was used to read data from the HTS221 device.
  - This uses polling
- `HAL_I2C_Mem_Write` was used to write data to the HTS221 device
  - This uses polling

# TODO,

- [ ] Test HAL I2C Interrupt functions
