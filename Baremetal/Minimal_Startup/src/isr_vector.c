// Typedefs
typedef void (*isr_void_fn)(void);

// Extern Variables
extern void *__istack;

// Extern Functions
extern int main(void);

// Static Functions
static void reset_handler(void);

const isr_void_fn isr_vector[240] __attribute__((section(".isr_vector"))) = {
    (void *)(&__istack), /* Initial Stack Pointer */
    reset_handler,       /* Reset Handler */
};

static void reset_handler(void) {

  // Copy .data from flash to ram

  // Initialize .bss section to 0

  // Call the main function
  main();
}
