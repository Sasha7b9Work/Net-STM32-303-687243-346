// 2022/04/27 15:11:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include <stm32f1xx_hal.h>


HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef * /*RCC_ClkInitStruct*/, uint32_t /*FLatency*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef * /*RCC_OscInitStruct*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef * /*PeriphClkInit*/)
{
    return HAL_OK;
}


void HAL_RCC_GetOscConfig(RCC_OscInitTypeDef * /*RCC_OscInitStruct*/)
{

}