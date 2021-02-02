/**
 * NOTE: _sbrk increments bytes not words
 */
#include <sys/types.h>

// This flag is used if we want to avoid using malloc based functions
#define GENERATE_LINK_ERROR_ON_MALLOC 0

#if GENERATE_LINK_ERROR_ON_MALLOC == 1
void unimplemented_function();
#endif

// https://tty.uchuujin.de/2016/04/stm32-from-scratch-heap/
// https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2019/11/06/malloc_and_sbrk-Qowo
// NOTE, Cast to `char *` or `uint8_t *` since increment only adds it by 1
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

  // We are comparing addresses here
  if ((size_t)(heap_end + increment) > (size_t)heap_limit) {
    return NULL; // error - no more memory
  }

  heap_end += increment;
  return (caddr_t)prev_heap_end;
}
