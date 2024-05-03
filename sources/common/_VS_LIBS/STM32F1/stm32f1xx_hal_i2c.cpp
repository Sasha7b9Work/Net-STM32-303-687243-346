// 2022/04/27 15:03:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include <stm32f1xx_hal.h>


HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef * /*hi2c*/)
{
    return HAL_OK;
}


HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef * /*hi2c*/)
{
    return HAL_I2C_STATE_READY;
}


HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef * /*hi2c*/, uint16_t /*DevAddress*/, uint16_t /*MemAddress*/,
    uint16_t /*MemAddSize*/, uint8_t * /*pData*/, uint16_t /*Size*/, uint32_t /*Timeout*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef * /*hi2c*/, uint16_t /*DevAddress*/, uint16_t /*MemAddress*/,
    uint16_t /*MemAddSize*/, uint8_t * /*pData*/, uint16_t /*Size*/, uint32_t /*Timeout*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef * /*hi2c*/, uint16_t /*DevAddress*/, uint8_t * /*pData*/,
    uint16_t /*Size*/, uint32_t /*Timeout*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef * /*hi2c*/, uint16_t /*DevAddress*/, uint8_t * /*pData*/,
    uint16_t /*Size*/, uint32_t /*Timeout*/)
{
    return HAL_OK;
}
