#include "tx_loop.h"


volatile struct uart_buffer uart_buffer;

/**
  * @brief Called at each UART byte receive from IRQ
  * @retval None
  */
void tic_rx_callback(void)
{
  uint32_t isrflags   = READ_REG((&huart1)->Instance->ISR);
  uint32_t cr1its     = READ_REG((&huart1)->Instance->CR1);

  __HAL_UART_CLEAR_FLAG(&huart1,
                        UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);


  if (((isrflags & USART_ISR_RXNE) != 0)
      && ((cr1its & USART_CR1_RXNEIE) != 0)) {
    if (uart_buffer.count < UART_BUFFER_SIZE) {
      uart_buffer.buffer[uart_buffer.head] = READ_REG((&huart1)->Instance->RDR) &
                                             0x7F;
      uart_buffer.head = (uart_buffer.head + 1) % UART_BUFFER_SIZE;
      uart_buffer.count++;

    } else {
      __HAL_UART_SEND_REQ(&huart1, UART_RXDATA_FLUSH_REQUEST);
    }
  }
}

/**
  * @brief Read last saved AES-CTR counter from flash
  * @retval AES-CTR counter value
  */
static uint32_t get_counter(void)
{
  uint32_t counter = *(__IO uint32_t *)(FLASH_COUNTER_ADDR);
  return counter;
}

/**
  * @brief Save last used AES-CTR counter to flash
  * @retval None
  */
static void save_counter(uint32_t counter_value)
{
  HAL_FLASH_Unlock();

  FLASH_EraseInitTypeDef EraseInitStruct;
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = FLASH_COUNTER_ADDR;
  EraseInitStruct.NbPages     = 1;

  uint32_t err;

  HAL_FLASHEx_Erase(&EraseInitStruct, &err);

  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_COUNTER_ADDR, counter_value);

  HAL_FLASH_Lock();
}

/**
  * @brief Main loop for TX firmware
  * @retval None
  */
void tx_loop(void)
{
  struct AES_ctx aes_ctx;
  uint8_t aes_iv[16];
  uint32_t counter;

  struct ciphered_frame ciphered_frame;

  uint8_t status;

  /*
  AES context init
   */
  counter = get_counter();
  counter += 100000; // Increment, to be sure not to use the same counter twice
  save_counter(counter);

  memcpy(aes_iv, NONCE, 12);
  uint32_t block_counter = __builtin_bswap32(counter * BLOCKS_PER_FRAME);
  memcpy(aes_iv + 12, (uint8_t *)(&block_counter), sizeof(block_counter));

  AES_init_ctx_iv(&aes_ctx, AES_KEY, aes_iv);

  /*
  NRF24 configuration
   */
  uint8_t ADDR[] = NRF_RF_ADDR;
  nRF24_SetRFChannel(NRF_RF_CHANNEL);
  nRF24_SetDataRate(nRF24_DR_250kbps);
  nRF24_SetCRCScheme(nRF24_CRC_1byte);
  nRF24_SetAddrWidth(5);
  nRF24_SetAddr(nRF24_PIPETX, ADDR);
  nRF24_SetAddr(nRF24_PIPE0, ADDR);
  nRF24_SetTXPower(nRF24_TXPWR_0dBm);
  nRF24_SetAutoRetr(nRF24_ARD_2500us, NRF_RF_RETRY_COUNT);
  nRF24_EnableAA(nRF24_PIPE0);
  nRF24_SetOperationalMode(nRF24_MODE_TX);
  nRF24_SetPowerMode(nRF24_PWR_UP);

  uart_buffer.count = 0;
  uart_buffer.head = 0;
  uart_buffer.tail = 0;

  // Enable TIC UART RX IRQ
  SET_BIT((&huart1)->Instance->CR1, USART_CR1_RXNEIE);

  while (1) {
    while (uart_buffer.count < sizeof(ciphered_frame.data));

    for (int i = 0; i < sizeof(ciphered_frame.data); i++) {
      ciphered_frame.data[i] = uart_buffer.buffer[(uart_buffer.tail + i)  %
                               UART_BUFFER_SIZE];
    }

    uart_buffer.tail = (uart_buffer.tail + sizeof(ciphered_frame.data)) %
                       UART_BUFFER_SIZE;
    uart_buffer.count = uart_buffer.count - sizeof(ciphered_frame.data);

    ciphered_frame.counter = counter;
    AES_CTR_xcrypt_buffer(&aes_ctx, ciphered_frame.data,
                          sizeof(ciphered_frame.data));

    /*
    Periodically save the counter to prevent
    reuse in case the power is cut
     */
    if ((counter % 100000) == 0) {
      save_counter(counter);
    }

    counter++;

    nRF24_WritePayload((uint8_t *)(&ciphered_frame), sizeof(ciphered_frame));

    /*
    Send packet
     */
    nRF24_CE_H;

    while (1) {
      status = nRF24_GetStatus();

      if (status & (nRF24_FLAG_TX_DS | nRF24_FLAG_MAX_RT)) {
        break;
      }
    }

    nRF24_CE_L;

    status = nRF24_GetStatus();
    nRF24_ClearIRQFlags();
  }
}
