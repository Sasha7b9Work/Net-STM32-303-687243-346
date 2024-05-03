#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f3xx_hal.h>


static I2C_HandleTypeDef hi2c1;


namespace HAL_I2C1
{
    void *handle = &hi2c1;
}


void HAL_I2C1::Init(void)
{
    pinSCL.Init();
    pinSDA.Init();

    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x00B0DBFF;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
    }

    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_DISABLE) != HAL_OK)
    {
    }

    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
    }
}


int8 HAL_I2C1::Read(uint8 dev_id, uint8 reg_addr, uint8* reg_data, uint16 len)
{
    TimeMeterMS meter;

    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
        if (meter.ElapsedTime() > 10)
        {
            return -1;
        }
    }

    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, (uint16)((dev_id << 1) + 1), reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, len, 5);

    if (status == HAL_OK)
    {
        rslt = 0;
    }
    else
    {
        rslt = -1;
    }
    return rslt;
}


int8 HAL_I2C1::Read16(uint8 dev_id, uint8* data)
{
    TimeMeterMS meter;

    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
        if (meter.ElapsedTime() > 10)
        {
            return -1;
        }
    }

    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&hi2c1, (uint16)((dev_id << 1) + 1), data, 2, 10);

    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
    }

    return (status == HAL_OK) ? 0 : -1;
}


int8 HAL_I2C1::Write(uint8 dev_id, uint8 reg_addr, uint8* reg_data, uint16 len)
{
    TimeMeterMS meter;

    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
        if (meter.ElapsedTime() > 10)
        {
            return -1;
        }
    }
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    HAL_StatusTypeDef status = HAL_OK;
    status = HAL_I2C_Mem_Write(&hi2c1, (uint16)(dev_id << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, len, 0xffff);

    if (status == HAL_OK)
    {
        rslt = 0;
    }
    else
    {
        rslt = -1;
    }
    return rslt;
}


int8 HAL_I2C1::Write8(uint8 dev_id, uint8 data)
{
    TimeMeterMS meter;

    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
        if (meter.ElapsedTime() > 10)
        {
            return -1;
        }
    }

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, (uint16)(dev_id << 1), &data, 1, 10);

    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
    }

    return (status == HAL_OK) ? 0 : -1;
}
