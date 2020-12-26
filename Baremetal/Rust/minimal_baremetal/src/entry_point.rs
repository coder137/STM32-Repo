#[link_section = ".vector_table.reset_vector"]
#[no_mangle]
pub static RESET_VECTOR: unsafe extern "C" fn() -> ! = Reset;

// NOTE, All the externed modules come here
#[no_mangle]
pub unsafe extern "C" fn Reset() -> ! {
  extern "C" {
    // .data section
    static mut __data_end__: u8;
    static mut __data_start__: u8;
    static mut __etext: u8;

    // .bss section
    static mut __bss_start__: u8;
    static mut __bss_end__: u8;
  }

  // data
  let vma_data_end = &__data_end__ as *const u8;
  let vma_data_start = &__data_start__ as *const u8;
  let lma_data_start = &__etext as *const u8;
  let count: usize = vma_data_end as usize - vma_data_start as usize;
  // core::ptr::copy_nonoverlapping(lma_data_start, &mut __data_start__ as *mut u8, count);
  core::ptr::copy_nonoverlapping(lma_data_start, vma_data_start as *mut u8, count);

  // end
  let bss_end = &__bss_end__ as *const u8;
  let bss_start = &__bss_start__ as *const u8;
  let count = bss_end as usize - bss_start as usize;
  // core::ptr::write_bytes(&mut __bss_start__ as *mut u8, 0, count);
  core::ptr::write_bytes(bss_start as *mut u8, 0, count);

  extern "Rust" {
    fn main() -> !;
  }
  main();
}

pub union Vector {
  reserved: u32,
  handler: unsafe extern "C" fn(),
}

extern "C" {
  fn NMI();
  fn HardFault();
  fn MemManage();
  fn BusFault();
  fn UsageFault();
  fn SVCall();
  fn PendSV();
  fn SysTick();
}

#[link_section = ".vector_table.exceptions"]
#[no_mangle]
pub static EXCEPTIONS: [Vector; 14] = [
  Vector { handler: NMI },
  Vector { handler: HardFault },
  Vector { handler: MemManage },
  Vector { handler: BusFault },
  Vector {
    handler: UsageFault,
  },
  Vector { reserved: 0 },
  Vector { reserved: 0 },
  Vector { reserved: 0 },
  Vector { reserved: 0 },
  Vector { handler: SVCall },
  Vector { reserved: 0 },
  Vector { reserved: 0 },
  Vector { handler: PendSV },
  Vector { handler: SysTick },
];

// TODO, Add peripheral interrupts here

#[no_mangle]
pub extern "C" fn DefaultExceptionHandler() {
  loop {}
}
