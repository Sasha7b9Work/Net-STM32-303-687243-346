#include "defines.h"
#include <stm32f1xx_hal.h>
#include <ctime>


HAL_StatusTypeDef HAL_Init()
{
    return HAL_OK;
}


uint HAL_GetTick()
{
    return (uint)(clock());
}

void HAL_IncTick()
{
}


void HAL_Delay(uint time)
{
    uint end = HAL_GetTick() + time;

    while (HAL_GetTick() < end)
    {

    }
}
