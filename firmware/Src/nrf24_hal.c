#include "nrf24_hal.h"

/**
  * @brief Low level SPI transmit/receive function (hardware depended)
  * input:
  *   data - value to transmit via SPI
  * @retval  value received from SPI
  */
uint8_t nRF24_LL_RW(uint8_t data)
{
  uint8_t ret;
  HAL_SPI_TransmitReceive(&hspi1, &data, &ret, 1, HAL_MAX_DELAY);
  return ret;
}
