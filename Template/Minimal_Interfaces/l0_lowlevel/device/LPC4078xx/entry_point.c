extern void peripheral__initialize();
extern int main();

void _start(void) {
  // peripherals
  peripheral__initialize();

  // l5_application layer
  main();

  // Should not return
  while (1) {
  }
}
