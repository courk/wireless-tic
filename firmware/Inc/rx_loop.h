#ifndef __RX_LOOP_H
#define __RX_LOOP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "main.h"
#include "usart.h"
#include "nrf24.h"
#include "aes.h"
#include "ciphered_frame.h"


void rx_loop(void);

#ifdef __cplusplus
}
#endif

#endif /* __RX_LOOP_H */
