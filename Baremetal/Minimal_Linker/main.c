void __main(void);

__attribute__((section(".isr_vector"))) const void (*isr_vector[255])(void) = {
    (void *)0x20000400,
    (void *)__main,
    // TODO, Add more here
};

// COMMON section
int global_uninit;
const int const_uninit;

// data section
int global_init = 10;
static int static_init = 10;

// bss section
static int static_uninit;
static const int const_static_uninit;

// rodata section
const int const_init = 10;
static const int const_static_init = 10;

void __main(void)
{
}
