- [Newlib Syscalls](#newlib-syscalls)
  - [File descriptors](#file-descriptors)
- [General stubs](#general-stubs)
  - [Code Snippet](#code-snippet)
- [printf specific requirements](#printf-specific-requirements)
  - [Explanation](#explanation)
  - [Code Snippet](#code-snippet-1)
- [scanf specific requirements](#scanf-specific-requirements)
  - [Explanation](#explanation-1)
  - [Code Snippet](#code-snippet-2)

# Newlib Syscalls

Using `printf` and `scanf` in our program

## File descriptors

```c
#include <unistd.h> // see the file descriptors number
```

- `stdin` == 0
- `stdout` == 1
- `stderr` == 2

# General stubs

- These are the stubs required by `printf`, `scanf` based functions

## Code Snippet

```c
#include <sys/stat.h>

int _close(int file) { return -1; }

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file) { return 1; }

int _lseek(int file, int ptr, int dir) { return 0; }
```

# printf specific requirements

## Explanation

- Calls the `uart_interrupt_` version of code irrespective of wether file descriptor is `stdout` or `stderr`
- We can always use a `uart__` based polling function for `stderr` for debugging inside interrupt functions
  - NOTE, Do not use `uart_interrupt_` based printf inside an interrupt
  - Read up on **FreeRTOS Context Switching** to know more
- Returns the length of the data that is transmitted

## Code Snippet

```c
extern UART_interrupt_s uart_interrupt_config;

int _write(int file, char *ptr, int len) {
  uart_interrupt__write_string_n(&uart_interrupt_config, ptr, len);
  return len;
}
```

# scanf specific requirements

## Explanation

- We read only one character at a time and return it back to the user
- The `_read` function is called the appropriate amount of times to receive all the data via `scanf`
  - NOTE, `scanf` can be easily cause massive errors in our code
  - For example: `scanf("%d", &expects_integer);`
  - If we input a character `scanf` will keep returning 0 till that character isnt **flushed**
  - Better to always get a string `%s` and then parse the string yourself


## Code Snippet

```c
extern UART_interrupt_s uart_interrupt_config;

int _read(int file, char *ptr, int len) {
  *ptr = uart_interrupt__read(&uart_interrupt_config, portMAX_DELAY);
  return 1;
}
```
