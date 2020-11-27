/**
 * Documentation
 * Newlib Library: https://sourceware.org/newlib/libc.html
 * <unistd> contains the file no descriptors for stdin, stdout and stderr
 */

#include <stdio.h>
#include <sys/stat.h>

int _close(int file) { return -1; }

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file) { return 1; }

int _lseek(int file, int ptr, int dir) { return 0; }

// Check peripheral.c
// For _read and _write
