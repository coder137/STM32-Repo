#ifndef DRIVER_UART_UART_INTERRUPT_H_
#define DRIVER_UART_UART_INTERRUPT_H_

#include <stdint.h>

#include "uart.h"

#include "FreeRTOS.h"
#include "queue.h"

// platform specific
#include "uart/uart_interrupt_internal.h"

// USART Events
typedef enum {
  // TX
  UART_interrupt_event_SEND_FRAME_COMPLETE, // Send completed; however USART may
                                            // still transmit data
  UART_interrupt_event_TRANSFER_COMPLETE, // Complete data has been transmitted
  UART_interrupt_event_TX_COMPLETE,       // Queued to be sent

  // RX
  // TODO, Not used
  UART_interrupt_event_RECEIVE_COMPLETE,

  UART_interrupt_event_RX_OVERFLOW,
  UART_interrupt_event_RX_TIMEOUT,
  UART_interrupt_event_RX_BREAK,
  UART_interrupt_event_RX_FRAMING_ERROR,
  UART_interrupt_event_RX_PARITY_ERROR,
} UART_interrupt_event_e;

typedef struct {
  QueueHandle_t rx_queue;
  QueueHandle_t tx_queue;
} UART_interrupt_internal_s;

typedef struct {
  UART_s *uart_config;
  uint32_t rx_queue_length;
  uint32_t tx_queue_length;

  void (*UART_event_cb)(UART_interrupt_event_e);

  // Do not update this
  UART_interrupt_internal_s _internal;
} UART_interrupt_s;

void uart_interrupt__init(UART_interrupt_s *interrupt_config);

void uart_interrupt__write(UART_interrupt_s *interrupt_config, uint8_t data);
void uart_interrupt__write_string(UART_interrupt_s *interrupt_config,
                                  char *data);
void uart_interrupt__write_string_n(UART_interrupt_s *interrupt_config,
                                    char *data, size_t n);

uint8_t uart_interrupt__read(const UART_interrupt_s *interrupt_config,
                             uint32_t wait_for);

// Used by Interrupt Function
void uart_interrupt__process(const UART_interrupt_s *interrupt_config);

#endif // DRIVER_UART_UART_INTERRUPT_H_
