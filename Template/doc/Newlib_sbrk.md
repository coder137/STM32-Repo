- [Newlib Sbrk](#newlib-sbrk)
  - [Requirement](#requirement)
- [Generic _sbrk implementation](#generic-_sbrk-implementation)
  - [Explanation](#explanation)
  - [Code Snippet](#code-snippet)
- [_sbrk implementation with _HeapLimit](#_sbrk-implementation-with-_heaplimit)
  - [Explanation](#explanation-1)
  - [Code Snippet](#code-snippet-1)
- [Create linker errors on _sbrk usage](#create-linker-errors-on-_sbrk-usage)
  - [Explanation](#explanation-2)
  - [Code Snippet](#code-snippet-2)

# Newlib Sbrk

Usage of `_sbrk` in newlib for allocating memory on the heap

- `_sbrk` allocates `required` amount of data
- Returns a pointer to the previous_data_address
- Shifts the `heap_end` address by the `required` amount

## Requirement

- `_sbrk` is the main function that is called my `malloc` for memory based allocation
- What makes `_sbrk` different from the other system calls is that it uses the linker script `heap` based symbols to allocate memory

# Generic _sbrk implementation

- Newlib based stub function

## Explanation

- `_end` is the start of the Heap address space
  - The naming is called `_end` because it is the end of `.data` and `.bss` data
  - Heap starts after that
- `static char *heap_end` is used to keep track of your current heap end
  - This is used to increase the code space
- Check is made to see that we are not overflowing into the stack_ptr space

## Code Snippet

```c
caddr_t sbrk(int incr) {
  extern char _end;		/* Defined by the linker */
  static char *heap_end;
  char *prev_heap_end;
 
  if (heap_end == 0) {
    heap_end = &_end;
  }
  prev_heap_end = heap_end;
  if (heap_end + incr > stack_ptr) {
    write (1, "Heap and stack collision\n", 25);
    abort ();
  }

  heap_end += incr;
  return (caddr_t) prev_heap_end;
}
```



# _sbrk implementation with _HeapLimit

- In many linker scripts a _HeapLimit is placed
- This is done to restrict the amount of Heap and Stack space that can be used by the user
- A linker error is generated if we overflow the heap or the stack space
- Heap and stack space is allocated on RAM

## Explanation

- The difference here is that we also use the `__HeapLimit` symbol from the linker script
- In our allocation check, instead of checking if we overflow the `stack_address` we check to see if we go above the `heap_limit`
- Everything else is kept same as per the above example

## Code Snippet

```c
caddr_t _sbrk(int increment) {
  extern void *__end__;
  extern void *__HeapLimit;

  // This initializes only once
  // This is an important cast, since we only want to increment by 1 when
  // pointer increments
  static char *heap_end = (char *)&__end__;

  char *heap_limit = (char *)&__HeapLimit;
  char *prev_heap_end = heap_end;
  if ((heap_end + increment) > (char *)heap_limit) {
    return ((void *)-1); // error - no more memory
  }

  heap_end += increment;
  return (caddr_t)prev_heap_end;
}
```

# Create linker errors on _sbrk usage

## Explanation

- In many cases we might want to avoid dynamic allocation completely in a system
- When any function such as `malloc`, `sprintf`, `printf` are called, internally `_sbrk` is also called
  - Sometimes we might allocate a lot of memory on the heap without realizing this
- To safe guard against this we create an `unimplemented_function` and use it in our function body WITHOUT defining it
- If `_sbrk` is used internally in any function our binary will not compile
  - `_sbrk` is the only way to allocate memory in C
  - In C++ we might have to override the `new` and `new[]` operators as well depending on our implementation

## Code Snippet

```c
#define GENERATE_LINK_ERROR_ON_MALLOC 1

// Do not define this function
void unimplemented_function(void);

caddr_t _sbrk(int increment) {
#if GENERATE_LINK_ERROR_ON_MALLOC == 1
  unimplemented_function();
#endif

  extern void *__end__;
  extern void *__HeapLimit;

  // This initializes only once
  // This is an important cast, since we only want to increment by 1 when
  // pointer increments
  static char *heap_end = (char *)&__end__;

  char *heap_limit = (char *)&__HeapLimit;
  char *prev_heap_end = heap_end;
  if ((heap_end + increment) > (char *)heap_limit) {
    return ((void *)-1); // error - no more memory
  }

  heap_end += increment;
  return (caddr_t)prev_heap_end;
}
```
