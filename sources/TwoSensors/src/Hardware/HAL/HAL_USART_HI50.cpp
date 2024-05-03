// 2024/04/30 11:27:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/RingBuffer.h"
#include <stm32f3xx_hal.h>


namespace HAL_USART_HI50
{
    /*
    *   USART1 RX - PB7
    *   USART1 TX - PB6
    */

    RingBuffer<256> recv_buffer;

    static UART_HandleTypeDef handleUART;

    void *handle = &handleUART;

    static uint8 recv_byte = 0;

    static void (*callback_on_receive)(pchar) = nullptr;
}


void HAL_USART_HI50::Init(void (*_callback_on_receive)(pchar))
{
    __HAL_RCC_USART1_CLK_ENABLE();

    callback_on_receive = _callback_on_receive;

    GPIO_InitTypeDef is;
    //           TX           RX
    is.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    is.Mode = GPIO_MODE_AF_PP;
    is.Alternate = GPIO_AF7_USART1;
    is.Speed = GPIO_SPEED_FREQ_HIGH;
    is.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOB, &is);

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

    if (HAL_UART_Init(&handleUART) != HAL_OK)
    {
        HAL::ErrorHandler();
    }

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    if (HAL_UART_Receive_IT(&handleUART, (uint8 *)&recv_byte, 1) != HAL_OK)
    {
        HAL::ErrorHandler();
    }
}


void HAL_USART_HI50::Send(uint8 byte)
{
    HAL_UART_Transmit(&handleUART, &byte, 1, 10);
}


void HAL_USART_HI50::ReceiveCallback(uint8 byte)
{
    static int counter = 0;
    
    if(++counter == 2)
    {
        counter = counter;
    }
    
    recv_buffer.Append(byte);

    if (HAL_UART_Receive_IT(&handleUART, &recv_byte, 1) != HAL_OK)
    {
        HAL::ErrorHandler();
    }
}


void HAL_USART_HI50::Update()
{
    if (!callback_on_receive || recv_buffer.GetElementCount() == 0)
    {
        return;
    }

    static Buffer<256> buffer;

    uint8 byte = recv_buffer.Pop();

    if (byte == 0x0d)
    {
        return;
    }

    if (byte == 0x0a)
    {
        buffer.Push(0);

        callback_on_receive((pchar)buffer.DataConst());

        buffer.Clear();

        return;
    }

    buffer.Push(byte);
}
