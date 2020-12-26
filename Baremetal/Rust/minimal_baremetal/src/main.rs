#![no_main]
#![no_std]

mod entry_point;
mod rust_entry_point;

mod blink;

#[no_mangle]
fn main() -> ! {
    blink::blink_init();
    blink::blink_set();
    loop {
        blink::_spin_delay(100_000);
        blink::blink_reset();
        blink::_spin_delay(100_000);
        blink::blink_set();
    }
}
