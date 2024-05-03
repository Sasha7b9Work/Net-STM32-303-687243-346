// 2022/04/27 14:59:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include <stm32f1xx_hal.h>


HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef * /*huart*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef * /*huart*/, uint8_t * /*pData*/, uint16_t /*Size*/, uint32_t /*Timeout*/)
{
    return HAL_OK;
}


void HAL_UART_IRQHandler(UART_HandleTypeDef *)
{

}


void HAL_NVIC_SetPriority(IRQn_Type, uint32_t, uint32_t )
{

}


void HAL_NVIC_EnableIRQ(IRQn_Type)
{

}


HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *, uint8_t *, uint16_t)
{
    return HAL_OK;
}
