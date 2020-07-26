#include <stdint.h>

// Typedefs
typedef void (*isr_void_fn)(void);

// Extern Variables
extern void *__istack;

// Extern Functions
extern int main(void);

// Static Functions
static void reset_handler(void);
static void initialize_data_section(void);
static void initialize_bss_section(void);

const isr_void_fn isr_vector[240] __attribute__((section(".isr_vector"))) = {
    (void *)(&__istack), /* Initial Stack Pointer */
    reset_handler,       /* Reset Handler */
};

static void reset_handler(void) {
  // Copy .data from flash to ram
  initialize_data_section();

  // Initialize .bss section to 0
  initialize_bss_section();

  // Call the main function
  main();
}

/**
 * @brief Copy data from the LMA (Flash) to the VMA (RAM)
 *
 */
static void initialize_data_section(void) {
  extern void *_sdata;
  extern void *_edata;
  extern void *_lma_data;

  const uint8_t *end_of_vma_data = (uint8_t *)_edata;
  const uint8_t *start_of_vma_data = (uint8_t *)_sdata;    // vma
  const uint8_t *start_of_lma_data = (uint8_t *)_lma_data; // lma

  const uint32_t size = end_of_vma_data - start_of_vma_data;
  for (uint32_t i = 0; i < size; i++) {
    start_of_vma_data = start_of_lma_data;
    start_of_vma_data++;
    start_of_lma_data++;
  }
}

static void initialize_bss_section(void) {
  extern void *_sbss;
  extern void *_ebss;

  uint8_t *ptr = (uint8_t *)&_sbss;
  while (ptr < (uint8_t *)&_ebss) {
    *ptr = 0;
    ptr++;
  }
}
