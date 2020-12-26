#ifndef LIBRARIES_DEVICE_H_
#define LIBRARIES_DEVICE_H_

// We cannot directly use __NOP from within rust
// Rust can only interface with C functions (not macros or inline assembly)
void c_asm_nop(void);

#endif
