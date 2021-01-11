use core::panic::PanicInfo;

#[panic_handler]
pub fn panic(_panic: &PanicInfo<'_>) -> ! {
  loop {}
}
