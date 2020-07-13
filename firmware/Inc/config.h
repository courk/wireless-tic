#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define NRF_RF_ADDR { 'w', 'i', 't', 'i', 'c' }
#define NRF_RF_CHANNEL 90
#define NRF_RF_RETRY_COUNT 10

#define AES_KEY (uint8_t *)"xxxxxxxxxxxxxxxx"
#define NONCE (uint8_t *)"xxxxxxxxxxxx"

#define FLASH_COUNTER_ADDR 0x8003c00

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */
