// 2022/04/27 14:36:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include <stm32f1xx_hal.h>


void  HAL_GPIO_Init(GPIO_TypeDef * /*GPIOx*/, GPIO_InitTypeDef * /*GPIO_Init*/)
{

}


void  HAL_GPIO_DeInit(GPIO_TypeDef * /*GPIOx*/, uint32_t /*GPIO_Pin*/)
{

}


void HAL_GPIO_WritePin(GPIO_TypeDef * /*GPIOx*/, uint16_t /*GPIO_Pin*/, GPIO_PinState /*PinState*/)
{

}


GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef * /*GPIOx*/, uint16_t /*GPIO_Pin*/)
{
    return GPIO_PIN_SET;
}
