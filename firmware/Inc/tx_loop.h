#ifndef __TX_LOOP_H
#define __TX_LOOP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "main.h"
#include "usart.h"
#include "nrf24.h"
#include "aes.h"
#include "ciphered_frame.h"

#define UART_BUFFER_SIZE 256

struct uart_buffer {
  int head;
  int tail;
  int count;
  uint8_t buffer[UART_BUFFER_SIZE];
};

void tic_rx_callback(void);
void tx_loop(void);

#ifdef __cplusplus
}
#endif

#endif /* __TX_LOOP_H */
