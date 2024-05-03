// 2023/11/30 09:02:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"


namespace HAL_USART2
{
    static UART_HandleTypeDef handleUART;

    void *handle = (void *)&handleUART;

    char recv_byte = 0;
}


void HAL_USART2::Init()
{
    pinTX_NEO_8M.Init();
    pinRX_NEO_8M.Init();

    handleUART.Instance = USART2;
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

    HAL_NVIC_SetPriority(USART2_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    HAL_UART_Receive_IT(&handleUART, (uint8 *)&recv_byte, 1);
}


void HAL_USART2::ReInit()
{
    HAL_UART_DeInit(&handleUART);

    Init();
}
