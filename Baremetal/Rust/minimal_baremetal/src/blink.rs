#[allow(non_snake_case)]
#[repr(C)]
struct RCC_TypeDef {
  pub CR: u32,
  pub ICSCR: u32,
  pub CFGR: u32,
  pub PLLCFGR: u32,
  pub PLLSAI1CFGR: u32,
  pub PLLSAI2CFGR: u32,
  pub CIER: u32,
  pub CIFR: u32,
  pub CICR: u32,
  pub RESERVED0: u32,
  pub AHB1RSTR: u32,
  pub AHB2RSTR: u32,
  pub AHB3RSTR: u32,
  pub RESERVED1: u32,
  pub APB1RSTR1: u32,
  pub APB1RSTR2: u32,
  pub APB2RSTR: u32,
  pub RESERVED2: u32,
  pub AHB1ENR: u32,
  pub AHB2ENR: u32,
  pub AHB3ENR: u32,
  pub RESERVED3: u32,
  pub APB1ENR1: u32,
  pub APB1ENR2: u32,
  pub APB2ENR: u32,
  pub RESERVED4: u32,
  pub AHB1SMENR: u32,
  pub AHB2SMENR: u32,
  pub AHB3SMENR: u32,
  pub RESERVED5: u32,
  pub APB1SMENR1: u32,
  pub APB1SMENR2: u32,
  pub APB2SMENR: u32,
  pub RESERVED6: u32,
  pub CCIPR: u32,
  pub RESERVED7: u32,
  pub BDCR: u32,
  pub CSR: u32,
}

#[allow(non_snake_case)]
#[repr(C)]
struct GPIO_TypeDef {
  pub MODER: u32,
  pub OTYPER: u32,
  pub OSPEEDR: u32,
  pub PUPDR: u32,
  pub IDR: u32,
  pub ODR: u32,
  pub BSRR: u32,
  pub LCKR: u32,
  pub AFR0: u32,
  pub AFR1: u32,
  pub BRR: u32,
  pub ASCR: u32,
}

extern "C" {
  pub fn c_asm_nop();
}

pub fn _spin_delay(delay: u32) {
  let mut mdelay = delay;
  while mdelay != 0 {
    unsafe { c_asm_nop() };
    mdelay -= 1;
  }
}

const PERIPH_BASE: u32 = 0x4000_0000;

const AHB1PERIPH_BASE: u32 = PERIPH_BASE + 0x0002_0000;
const RCC_BASE: u32 = AHB1PERIPH_BASE + 0x1000;

const AHB2PERIPH_BASE: u32 = PERIPH_BASE + 0x0800_0000;
const GPIOA_BASE: u32 = AHB2PERIPH_BASE + 0x0000;

#[allow(non_snake_case)]
pub fn blink_init() {
  //   RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  //   GPIOA->BRR |= (1 << 5); // Reset the pin here

  // Set the mode
  //   GPIOA->MODER &= ~(3 << 10);
  //   GPIOA->MODER |= (1 << 10); // 01 00 00 00 00 00

  // Check these registers
  //   GPIOA->OTYPER &= ~(1 << 5); // set to 0
  //   GPIOA->OSPEEDR &= ~(3 << 10);
  //   GPIOA->PUPDR &= ~(3 << 10);
  let safe_rcc = RCC_BASE as *mut RCC_TypeDef;
  let rcc = unsafe { &mut *(safe_rcc) };

  let safe_gpioa = GPIOA_BASE as *mut GPIO_TypeDef;
  let gpioa = unsafe { &mut *(safe_gpioa) };

  // activate GPIOA
  let mut rcc_ahb2enr = unsafe { core::ptr::read_volatile(&mut rcc.AHB2ENR) };
  rcc_ahb2enr |= 1 << 0;
  unsafe { core::ptr::write_volatile(&mut rcc.AHB2ENR, rcc_ahb2enr) };

  // MODER
  let mut gpioa_moder = unsafe { core::ptr::read_volatile(&mut gpioa.MODER) };
  gpioa_moder &= !(3 << 10);
  gpioa_moder |= 1 << 10;
  unsafe { core::ptr::write_volatile(&mut gpioa.MODER, gpioa_moder) };

  // OTYPER
  let mut gpioa_otyper = unsafe { core::ptr::read_volatile(&mut gpioa.OTYPER) };
  gpioa_otyper &= !(1 << 5);
  unsafe { core::ptr::write_volatile(&mut gpioa.OTYPER, gpioa_otyper) };

  // OSPEEDR
  let mut gpioa_ospeedr = unsafe { core::ptr::read_volatile(&mut gpioa.OSPEEDR) };
  gpioa_ospeedr &= !(3 << 10);
  unsafe { core::ptr::write_volatile(&mut gpioa.OSPEEDR, gpioa_ospeedr) };

  // PUPDR
  let mut gpioa_pupdr = unsafe { core::ptr::read_volatile(&mut gpioa.PUPDR) };
  gpioa_pupdr &= !(3 << 10);
  unsafe { core::ptr::write_volatile(&mut gpioa.PUPDR, gpioa_pupdr) };
}

pub fn blink_set() {
  // Set the pin here
  //   GPIOA->BSRR |= (1 << 5);
  let safe_gpioa = GPIOA_BASE as *mut GPIO_TypeDef;
  let gpioa = unsafe { &mut *(safe_gpioa) };

  let mut gpioa_bsrr = unsafe { core::ptr::read_volatile(&mut gpioa.BSRR) };
  gpioa_bsrr |= 1 << 5;
  unsafe { core::ptr::write_volatile(&mut gpioa.BSRR, gpioa_bsrr) };
}

pub fn blink_reset() {
  //     _spin_delay(1000 * 1000);
  //     GPIOA->BRR = (1 << 5); // Reset
  let safe_gpioa = GPIOA_BASE as *mut GPIO_TypeDef;
  let gpioa = unsafe { &mut *(safe_gpioa) };

  let mut gpioa_brr = unsafe { core::ptr::read_volatile(&mut gpioa.BRR) };
  gpioa_brr |= 1 << 5;
  unsafe { core::ptr::write_volatile(&mut gpioa.BRR, gpioa_brr) };
}
