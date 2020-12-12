#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "exti/exti.h"
#include "gpio/gpio.h"

#include "uart/uart.h"
#include "uart/uart_interrupt.h"

// ThreadX include
#include "tx_api.h"

#define DEMO_STACK_SIZE 1024
#define DEMO_BYTE_POOL_SIZE 9120
#define DEMO_BLOCK_POOL_SIZE 100
#define DEMO_QUEUE_SIZE 100

// ThreadX Control blocks
static TX_THREAD thread_0;
static TX_BYTE_POOL byte_pool_0;
static UCHAR memory_area[DEMO_BYTE_POOL_SIZE];
static ULONG thread_0_counter;

// UART
static UART_s uart_config;

// Static functions
static void thread_0_entry(ULONG thread_input);
static void main__uart_init(void);

int main() {
  main__uart_init();

  char buffer[20] = {0};
  sprintf(buffer, "s: %lu\r\n", SystemCoreClock);
  uart__write_string(&uart_config, buffer);

  /* Enter the ThreadX kernel.  */
  tx_kernel_enter();
}

/* Define what the initial system looks like.  */
void tx_application_define(void *first_unused_memory) {
  // ? debugging

  // Start the example here
  CHAR *pointer = TX_NULL;

  /* Create a byte memory pool from which to allocate the thread stacks.  */
  tx_byte_pool_create(&byte_pool_0, "byte pool 0", memory_area,
                      DEMO_BYTE_POOL_SIZE);

  /* Put system definition stuff in here, e.g. thread creates and other assorted
     create information.  */

  /* Allocate the stack for thread 0.  */
  tx_byte_allocate(&byte_pool_0, (VOID **)&pointer, DEMO_STACK_SIZE,
                   TX_NO_WAIT);

  /* Create the main thread.  */
  tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0, pointer,
                   DEMO_STACK_SIZE, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* Release the block back to the pool.  */
  tx_block_release(pointer);
}

/* Define the test threads.  */

static void thread_0_entry(ULONG thread_input) {
  char buffer[20] = {0};
  while (1) {

    /* Increment the thread counter.  */
    thread_0_counter++;

    // * Printing the data from the task
    sprintf(buffer, "counter: %ld\r\n", thread_0_counter);
    uart__write_string(&uart_config, buffer);

    /* Sleep for 10 ticks.  */
    tx_thread_sleep(10);
  }
}

static void main__uart_init(void) {
  // Activate USART1
  RCC->APB2ENR |= (1 << 14);
  // Activate GPIOB
  RCC->AHB2ENR |= (1 << 1);

  GPIO_s config = {};
  config.mode = GPIO_mode_ALTERNATE_FUNCTION;
  config.type = GPIO_type_PUSH_PULL;
  config.speed = GPIO_speed_VERY_HIGH_SPEED;
  config.pull = GPIO_pull_NO_PULLUP_OR_PULLDOWN;
  config.alternate = GPIO_alternate_7;
  gpio__init(&config, GPIOB, 6);
  gpio__init(&config, GPIOB, 7);

  uart_config.baud_rate = 115200U;
  uart_config.stop_bit = UART_stop_bit_1_0;
  uart_config.word_length = UART_word_length_8;
  uart_config.mode = UART_mode_RX_TX;
  uart__init(&uart_config, USART1);
}
