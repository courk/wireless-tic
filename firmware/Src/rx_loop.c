#include "rx_loop.h"


/**
  * @brief Main loop for RX firmware
  * @retval None
  */
void rx_loop(void)
{
  struct AES_ctx aes_ctx;

  struct ciphered_frame ciphered_frame;
  uint8_t aes_iv[16];
  uint8_t payload_length;

  /*
  AES context init
   */
  AES_init_ctx(&aes_ctx, AES_KEY);
  memcpy(aes_iv, NONCE, 12);

  /*
  NRF24 configuration
   */
  uint8_t ADDR[] = NRF_RF_ADDR;
  nRF24_SetRFChannel(NRF_RF_CHANNEL);
  nRF24_SetDataRate(nRF24_DR_250kbps);
  nRF24_SetCRCScheme(nRF24_CRC_1byte);
  nRF24_SetAddrWidth(5);
  nRF24_SetAddr(nRF24_PIPE1, ADDR);
  nRF24_SetRXPipe(nRF24_PIPE1, nRF24_AA_ON, 32);
  nRF24_SetTXPower(nRF24_TXPWR_0dBm);
  nRF24_SetOperationalMode(nRF24_MODE_RX);
  nRF24_SetPowerMode(nRF24_PWR_UP);

  nRF24_CE_H; // Start RX PHY

  while (1) {
    if (nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY) {
      nRF24_ReadPayload((uint8_t *)(&ciphered_frame), &payload_length);
      nRF24_ClearIRQFlags();

      uint32_t block_counter = __builtin_bswap32(ciphered_frame.counter *
                               BLOCKS_PER_FRAME);
      memcpy(aes_iv + 12, (uint8_t *)(&(block_counter)), sizeof(block_counter));

      AES_ctx_set_iv(&aes_ctx, aes_iv);

      AES_CTR_xcrypt_buffer(&aes_ctx, ciphered_frame.data, FRAME_DATA_SIZE);

      HAL_UART_Transmit(&huart1, ciphered_frame.data, FRAME_DATA_SIZE, HAL_MAX_DELAY);
    }
  }
}
