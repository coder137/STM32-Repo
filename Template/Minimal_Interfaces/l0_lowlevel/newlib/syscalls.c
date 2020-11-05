/**
 * Documentation
 * Newlib Library: https://sourceware.org/newlib/libc.html
 * <unistd> contains the file no descriptors for stdin, stdout and stderr
 */

#include <stdio.h>
#include <sys/stat.h>

#include "uart_interrupt.h"

extern UART_interrupt_s uart_interrupt_config;

int _close(int file) { return -1; }

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file) { return 1; }

int _lseek(int file, int ptr, int dir) { return 0; }

// TODO, Should be externed since it uses l3/l4 layer
int _read(int file, char *ptr, int len) {
  *ptr = uart_interrupt__read(&uart_interrupt_config, portMAX_DELAY);
  return 1;
}

// TODO, Should be externed since it uses l3/l4 layer
int _write(int file, char *ptr, int len) {
  uart_interrupt__write_string_n(&uart_interrupt_config, ptr, len);
  return len;
}
