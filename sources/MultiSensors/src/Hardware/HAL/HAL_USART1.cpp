// 2024/04/30 11:27:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/RingBuffer.h"
#include "Hardware/Timer.h"
#include "Modules/HI50/HI50.h"
#include "Modules/HC12/HC12.h"
#include "Modules/Mipex02/Mipex02.h"
#include <stm32f3xx_hal.h>


namespace HAL_USART1
{
    // Для чего используется USART1
    struct ModeUSART1
    {
        enum E
        {
            HC12,       // Для работы с HC12
            Sensor      // Для работы с датчиком
        };

        ModeUSART1(E v) : value(v) { }

        bool IsHC12() const
        {
            return value == HC12;
        }

    private:

        E value;
    };

    RingBuffer<256> recv_buffer;

    static UART_HandleTypeDef handleUART =
    {
        USART1,
        {
            19200,
            UART_WORDLENGTH_8B,
            UART_STOPBITS_1,
            UART_PARITY_NONE,
            UART_MODE_TX_RX,
            UART_HWCONTROL_NONE,
            UART_OVERSAMPLING_16,
            UART_ONE_BIT_SAMPLE_DISABLE
        },
        {
            UART_ADVFEATURE_NO_INIT
        }
    };

    void *handle = &handleUART;

    static uint8 recv_byte = 0;

    static void (*callback_on_receive)(pchar) = nullptr;
    static void (*callback_on_sensor)(pchar) = nullptr;

    static void Init(const ModeUSART1 &);
}


void HAL_USART1::Init(void (*_callback_on_receive_HI50)(pchar))
{
    recv_buffer.Clear();

    callback_on_sensor = _callback_on_receive_HI50;

    handleUART.Init.BaudRate = 19200;

    HAL_UART_Init(&handleUART);
}


void HAL_USART1::Init(const ModeUSART1 &mode)
{
    /*
    *   USART1 RX - PB7     // На датчики -
    *   USART1 TX - PB6     // идут там же, где I2C
    *
    *   PA9, PA10           // На HC-12
    */

    recv_buffer.Clear();

    if (mode.IsHC12())
    {
        if(HI50::IsExist() || Mipex02::IsExist())       // Деинициализируем данные выводы только если существует лазерный дальномер или Mipex02
        {                                               // В остальных случаях на этих выводах I2C - их отключать нельзя
            HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
            HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
        }
    }
    else
    {
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);
    }

    GPIO_InitTypeDef is =
    {
        //                                                           TX           RX
        mode.IsHC12() ? (uint)(GPIO_PIN_9 | GPIO_PIN_10) : (uint)(GPIO_PIN_6 | GPIO_PIN_7),
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_HIGH,
        GPIO_AF7_USART1
    };

    HAL_GPIO_Init(mode.IsHC12() ? GPIOA : GPIOB, &is);

    if (mode.IsHC12())
    {
        handleUART.Init.BaudRate = 9600;
    }
    else
    {
        if (HI50::IsExist())
        {
            handleUART.Init.BaudRate = 19200;
        }
        else if (Mipex02::IsExist())
        {
            handleUART.Init.BaudRate = 9600;
        }
    }

    if (HAL_UART_Init(&handleUART) != HAL_OK)
    {
        HAL::ErrorHandler();
    }

    if (mode.IsHC12())
    {
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
    else
    {
        HAL_NVIC_EnableIRQ(USART1_IRQn);

        HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);

        HAL_UART_Receive_IT(&handleUART, (uint8 *)&recv_byte, 1);
    }

    callback_on_receive = mode.IsHC12() ? nullptr : callback_on_sensor;
}


void HAL_USART1::SetModeHC12()
{
    Init(ModeUSART1::HC12);

    HC12::self.Init();
}


void HAL_USART1::SetModeSensor()
{
    Init(ModeUSART1::Sensor);
}


void HAL_USART1::Send(uint8 byte)
{
    HAL_UART_Transmit(&handleUART, &byte, 1, 10);
}


void HAL_USART1::Send(const void *buffer, int size)
{
    HAL_UART_Transmit(&handleUART, (const uint8 *)buffer, (uint16)size, 100);
}


void HAL_USART1::SendString(pchar message)
{
    Send(message, (int)std::strlen(message));
}


void HAL_USART1::ReceiveCallback()
{
    recv_buffer.Append(recv_byte);

    HAL_UART_Receive_IT(&handleUART, &recv_byte, 1);
}


void HAL_USART1::Update()
{
    if (!callback_on_receive)
    {
        recv_buffer.Clear();
        return;
    }

    if (recv_buffer.GetElementCount() == 0)
    {
        return;
    }

    while (recv_buffer.GetElementCount() != 0)
    {
        volatile uint8 byte = recv_buffer.Pop();

        if(HI50::IsExist())
        {
            if (byte == 0x0d)
            {
                return;
            }

            if (byte == 0x0a)
            {
                byte = 0x00;
            }
        }

        if(Mipex02::IsExist())
        {
            if (byte == 0x0d)
            {
                byte = 0x00;
            }
        }
        static char buffer[256] = { '\0' };
        static int pointer = 0;

        buffer[pointer++] = (char)byte;

        if(Mipex02::IsWaitMeasure())
        {
            if (buffer[0] == '@')
            {
                if (pointer >= 3)
                {
                    byte = 0x00;
                    buffer[pointer++] = (char)byte;
                }
            }
            else
            {
                pointer = 0;
            }
        }

        if (byte == 0x00)
        {
            callback_on_receive(buffer);

            pointer = 0;
        }
    }
}
