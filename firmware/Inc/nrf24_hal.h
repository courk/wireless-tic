#ifndef __NRF24_HAL_H
#define __NRF24_HAL_H


// Peripheral libraries
#include "spi.h"
#include "gpio.h"


#define nRF24_CE_L                 HAL_GPIO_WritePin(NRF_CE_GPIO_Port, NRF_CE_Pin, GPIO_PIN_RESET)
#define nRF24_CE_H                 HAL_GPIO_WritePin(NRF_CE_GPIO_Port, NRF_CE_Pin, GPIO_PIN_SET)

#define nRF24_CSN_L                HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)
#define nRF24_CSN_H                HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)

#define nRF24_RX_ON                nRF24_CE_H
#define nRF24_RX_OFF               nRF24_CE_L


#define nRF24_GPIO_Init(x)

// Function prototypes
uint8_t nRF24_LL_RW(uint8_t data);

#endif // __NRF24_HAL_H
