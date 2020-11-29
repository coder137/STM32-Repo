
#include "gpio.h"

extern int main();

void _start(void) {
  // l5_application layer
  main();

  while (1) {
  }
}

// ! Added for Testing
// ! Shift to l5_application layer later
// 96_000_000 cycles per second
// 96_000 cycles per millisecond
// 96 cycles per microsecond
static void delay_us(uint32_t us);
static void delay_ms(uint32_t ms);

int main() {
  GPIO_s config;
  config.mode = GPIO_mode_OUTPUT;
  gpio__init(&config, LPC_GPIO1, 18);

  while (1) {
    gpio__set(&config);
    delay_ms(100);
    gpio__reset(&config);
    delay_ms(100);
  }

  return 0;
}

static void delay_us(uint32_t us) {
  for (int i = 0; i < (96 * us); i++) {
    __NOP();
  }
}

static void delay_ms(uint32_t ms) {
  for (int i = 0; i < ms; i++) {
    delay_us(1000);
  }
}
