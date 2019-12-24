# L475_GPIO

HAL GPIO Example to

- Init Pin
- Write Pin
- Toggle Pin
- Read Pin

## Init Pin

[Read this article](https://embeddedartistry.com/blog/2018/06/04/demystifying-microcontroller-gpio-settings/)

### Open Drain vs Push Pull

- **Push-Pull** has the ability to be both a *current source* AND a *current sink*
- **Open-Drain** can only be a *current sink*
  - Open Drain
  - Open Drain with external Pullup

### Pullup vs Pulldown vs No Pull

- **Pullup** is connected to VCC
- **Pulldown** is connected to GND
- **No Pull** means no Pullup or Pulldown mode

### Speed

- Rate of change from high-low or low-high transitions
- By default this should be kept at a **slow speed**
  - **Power consumption** and **Noise radiated** increseases according to speed

