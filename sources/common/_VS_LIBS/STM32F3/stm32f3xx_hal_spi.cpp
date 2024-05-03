// 2022/04/27 14:33:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include <stm32f3xx_hal.h>


HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef * /*hspi*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef * /*hspi*/, uint8_t * /*pData*/, uint16_t /*Size*/, uint32_t /*Timeout*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef * /*hspi*/, uint8_t * /*pTxData*/, uint8_t * /*pRxData*/, uint16_t /*Size*/, uint32_t /*Timeout*/)
{
    return HAL_OK;
}
