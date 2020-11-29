#include "uart.h"

// STATIC FUNCTION DECLARATIONS
static void uart__disable(UART_s *config);
static void uart__enable(UART_s *config);

static void uart__set_word_length(UART_s *config);
static void uart__set_parity(UART_s *config);
static void uart__set_stop_bit(UART_s *config);
static void uart__set_baud_rate(UART_s *config);

static void uart__set_communication_mode(UART_s *config);

// Variables
static const uint8_t LCR_WLS = 0;
static const uint8_t LCR_SBS = 2;
static const uint8_t LCR_PE = 3;
static const uint8_t LCR_PS = 4;
static const uint8_t LCR_DLAB = 7;

static const uint8_t LSR_THRE = 5;

static const uint8_t FDR_MULVAL = 4;

static const uint8_t TER_TXEN = 7;

// Functions
void uart__init(UART_s *config, USART_TypeDef *usart) {
  config->usart = usart;

  uart__set_communication_mode(config);

  uart__set_baud_rate(config);

  uart__set_word_length(config);

  uart__set_stop_bit(config);

  uart__set_parity(config);

  uart__enable(config);
}

void uart__deinit(UART_s *config) { uart__disable(config); }

// NOTE, These APIS Read and Write directly to the register in a polling manner.
void uart__write(const UART_s *config, const char data) {
  // a) Check LSR for Transmit Hold Register Empty
  while (((config->usart->LSR >> LSR_THRE) & 0x01) == 0)
    ;

  // b) Copy output_byte to THR register
  config->usart->THR = data;
}

void uart__write_string(const UART_s *config, const char *buffer) {
  while (buffer != 0) {
    char data = *buffer;
    uart__write(config, data);
    buffer++;
  }
}

uint8_t uart__read(const UART_s *config) {
  // a) Check LSR for Receive Data Ready
  while ((config->usart->LSR & 0x01) == 0)
    ;

  // b) Copy data from RBR register to input_byte
  return config->usart->RBR;
}

// STATIC
static void uart__disable(UART_s *config) {
  config->usart->LCR |= (1 << LCR_DLAB);
}

static void uart__enable(UART_s *config) {
  config->usart->LCR &= ~(1 << LCR_DLAB);
}

static void uart__set_word_length(UART_s *config) {
  USART_TypeDef *usart = config->usart;
  uint32_t lcr_data = usart->LCR;
  lcr_data &= ~(3 << LCR_WLS);

  switch (config->word_length) {

  case UART_word_length_7:
    lcr_data |= (2 << LCR_WLS);
    break;
  case UART_word_length_5:
    lcr_data |= (0 << LCR_WLS);
    break;
  case UART_word_length_6:
    lcr_data |= (1 << LCR_WLS);
    break;
  case UART_word_length_8:
  case UART_word_length_9:
  default:
    lcr_data |= (3 << LCR_WLS);
    break;
  }

  usart->LCR = lcr_data;
}

static void uart__set_parity(UART_s *config) {
  uint8_t parity_enable = 0;
  uint8_t parity_select = 0;

  switch (config->parity) {
  case UART_parity_NONE:
    parity_enable = 0;
    break;
  case UART_parity_ODD:
    parity_enable = 1;
    parity_select = 0;
    break;
  case UART_parity_EVEN:
    parity_enable = 1;
    parity_select = 1;
    break;
  default:
    break;
  }

  uint32_t lcr_data = config->usart->LCR;
  lcr_data &= ~(3 << LCR_PS);
  lcr_data |= ((parity_enable << LCR_PE) | (parity_select << LCR_PS));
  config->usart->LCR = lcr_data;
}

static void uart__set_stop_bit(UART_s *config) {
  switch (config->stop_bit) {
  case UART_stop_bit_2_0:
    config->usart->LCR |= (1 << LCR_SBS);
    break;
  case UART_stop_bit_1_0:
  case UART_stop_bit_0_5:
  case UART_stop_bit_1_5:
  default:
    config->usart->LCR &= ~(1 << LCR_SBS);
    break;
  }
}

static void uart__set_baud_rate(UART_s *config) {
  uint16_t baud = SystemCoreClock / (16 * config->baud_rate);
  config->usart->LCR = (1 << LCR_DLAB); // Activate
  config->usart->DLM = (baud >> 8) & 0xFF;
  config->usart->DLL = (baud >> 0) & 0xFF;

  // DIVADDVAL = 0, MULVAL = 1
  config->usart->FDR = (1 << FDR_MULVAL);
}

static void uart__set_communication_mode(UART_s *config) {
  switch (config->communication_mode) {
  case UART_communication_mode_RX_ONLY:
  case UART_communication_mode_INACTIVE:
    config->usart->TER &= ~(1 << TER_TXEN);
    break;
  case UART_communication_mode_TX_ONLY:
  case UART_communication_mode_RX_TX:
  default:
    config->usart->TER |= (1 << TER_TXEN);
    break;
  }
}
