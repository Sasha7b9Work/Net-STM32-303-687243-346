// 2023/04/11 12:09:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f3xx_hal.h>


namespace HAL_USART1
{
    static UART_HandleTypeDef handleUART;

    void *handle = (void *)&handleUART;

    char recv_byte = 0;
}


void HAL_USART1::Init()
{
    pinTX_HC12.Init();
    pinRX_HC12.Init();

    handleUART.Instance = USART1;
    handleUART.Init.BaudRate = 9600;
    handleUART.Init.WordLength = UART_WORDLENGTH_8B;
    handleUART.Init.StopBits = UART_STOPBITS_1;
    handleUART.Init.Parity = UART_PARITY_NONE;
    handleUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    handleUART.Init.Mode = UART_MODE_TX_RX;
    handleUART.Init.OverSampling = UART_OVERSAMPLING_16;
    handleUART.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    handleUART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    HAL_UART_Init(&handleUART);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    HAL_UART_Receive_IT(&handleUART, (uint8 *)&recv_byte, 1);
}


void HAL_USART1::Transmit(const void *buffer, int size)
{
    if (!buffer)
    {
        return;
    }

    HAL_UART_Transmit(&handleUART, (uint8_t *)buffer, (uint16_t)size, 0xFFFF);
}
