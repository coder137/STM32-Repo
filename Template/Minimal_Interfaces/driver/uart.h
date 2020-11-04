#ifndef DRIVER_INTERFACE_USART_H_
#define DRIVER_INTERFACE_USART_H_

#include <stdint.h>

#include "gpio.h"

// platform specific
#include "uart/internal.h"

// https://github.com/ARM-software/CMSIS_5/blob/develop/CMSIS/Driver/Include/Driver_USART.h

// USART Events
typedef enum {
  ///< Send completed; however USART may still transmit data
  USART_event_SEND_COMPLETE = (1UL << 0),
  ///< Receive completed
  USART_event_RECEIVE_COMPLETE = (1UL << 1),
  ///< Transfer completed
  USART_event_TRANSFER_COMPLETE = (1UL << 2),
  ///< Transmit completed (optional)
  USART_event_TX_COMPLETE = (1UL << 3),
  ///< Transmit data not available (Synchronous Slave)
  USART_event_TX_UNDERFLOW = (1UL << 4),
  ///< Receive data overflow
  USART_event_RX_OVERFLOW = (1UL << 5),
  ///< Receive character timeout (optional)
  USART_event_RX_TIMEOUT = (1UL << 6),
  ///< Break detected on receive
  USART_event_RX_BREAK = (1UL << 7),
  ///< Framing error detected on receive
  USART_event_RX_FRAMING_ERROR = (1UL << 8),
  ///< Parity error detected on receive
  USART_event_RX_PARITY_ERROR = (1UL << 9),
  ///< CTS state changed (optional)
  USART_event_CTS = (1UL << 10),
  ///< DSR state changed (optional)
  USART_event_DSR = (1UL << 11),
  ///< DCD state changed (optional)
  USART_event_DCD = (1UL << 12),
  ///< RI  state changed (optional)
  USART_event_RI = (1UL << 13),
} usart_event_e;

// Callback signals

///< Pointer to \ref ARM_USART_SignalEvent : Signal USART Event
typedef void (*USART_signal_event_t)(usart_event_e event);

// config struct

// Functions

// ARM_USART_CAPABILITIES usart__get_capabilities(void);
// int32_t usart__power_control();
// ARM_USART_STATUS usart__get_status(void);
// void usart__set_modem_control();
// void usart__get_modem_status();

int32_t usart__initialize(USART_signal_event_t cb_event);
int32_t usart__uninitialize(void);

int32_t usart__send(const void *data, uint32_t num);
int32_t usart__receive(void *data, uint32_t num);
int32_t usart__transfer(const void *data_out, void *data_in);

uint32_t usart__get_tx_count(void);
uint32_t usart__get_rx_count(void);

int32_t usart__control(uint32_t control, uint32_t arg);

typedef enum {
  UART_word_length_8 = 0,
  UART_word_length_9,
  UART_word_length_7,
  UART_word_length_5,
  UART_word_length_6,
} UART_word_length_e;

typedef enum {
  UART_stop_bit_1_0,
  UART_stop_bit_0_5,
  UART_stop_bit_2_0,
  UART_stop_bit_1_5,
} UART_stop_bit_e;

typedef enum {
  UART_mode_INACTIVE,
  UART_mode_RX_ONLY,
  UART_mode_TX_ONLY,
  UART_mode_RX_TX
} UART_communication_mode_e;

typedef enum {
  UART_mode_ASYNCHRONOUS,
  UART_mode_SYNCHRONOUS_MASTER,
  UART_mode_SYNCHRONOUS_SLAVE,
  UART_mode_SINGLE_WIRE,
  UART_mode_IRDA,
  UART_mode_SMART_CARD,
} UART_operation_mode_e;

typedef enum {
  UART_parity_EVEN,
  UART_parity_NONE,
  UART_parity_ODD,
} UART_parity_e;

typedef enum {
  UART_flow_control_NONE,
  UART_flow_control_CTS,
  UART_flow_control_RTS,
  UART_flow_control_RTS_CTS,
} UART_flow_control_e;

// Clock Polarity
// Clock Phase

// Main Struct
typedef struct {
  USART_TypeDef *usart;
  GPIO_s rx;
  GPIO_s tx;

  UART_communication_mode_e communication_mode;
  UART_operation_mode_e operation_mode;
  UART_word_length_e word_length;
  UART_stop_bit_e stop_bit;
  UART_parity_e parity;
  UART_flow_control_e flow_control;

  uint32_t baud_rate;
} UART_s;

void uart__init(UART_s *config, USART_TypeDef *usart);
void uart__deinit(UART_s *config);

void uart__write(const UART_s *config, const char data);
void uart__write_string(const UART_s *config, const char *buffer);

// ! Error could take place in this function
// * OE (Overflow Error)
uint8_t uart__read(const UART_s *config);

#endif // DRIVER_UART_UART_H_
