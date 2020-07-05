#include <sys/stat.h>

#include "uart/uart_interrupt.h"

extern UART_interrupt_s uart_interrupt_config;

int _close(int file) { return -1; }

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file) { return 1; }

int _lseek(int file, int ptr, int dir) { return 0; }

int _read(int file, char *ptr, int len) { return 0; }

int _write(int file, char *ptr, int len) {
  uart_interrupt__write_string_n(&uart_interrupt_config, ptr, len);
  return len;
}
