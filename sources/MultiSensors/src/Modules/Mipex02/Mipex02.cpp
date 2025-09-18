// 2025/09/18 08:51:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/Mipex02/Mipex02.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Modules/HC12/HC12.h"
#include "Hardware/InterCom.h"


namespace Mipex02
{
    static bool is_exist = false;

    struct State
    {
        enum E
        {
            IDLE,
            WAIT_UART,      // Послали команду UART?, ждём ответа
            WAIT_MEASURE    // Послана команда периодических измерений, ждём их
                            // В этом режиме приходит ответ из трёх байт, где первый - @, и затем два байта измерений
        };
    };

    static State::E state = State::IDLE;

    static Measure concentrate;
}


bool Mipex02::IsWaitMeasure()
{
    return state == State::WAIT_MEASURE;
}


bool Mipex02::Init()
{
    is_exist = true;
    
    HAL_USART1::SetModeSensor();

    state = State::WAIT_UART;

    HAL_USART1::SendString("UART?\x0d");

    TimeMeterMS meter;

    while (meter.ElapsedTime() < 10000)
    {
        HAL_USART1::Update();

        if (state == State::WAIT_MEASURE)
        {
            break;
        }
    }
    
    is_exist = (state == State::WAIT_MEASURE);

    return IsExist();
}


bool Mipex02::IsExist()
{
    return is_exist;
}


void Mipex02::Update()
{

}


bool Mipex02::GetMeasure(Measure *measure)
{
    if (concentrate.correct)
    {
        *measure = concentrate;

        concentrate.Clear();

        return true;
    }

    return false;
}


void Mipex02::CallbackOnReceive(pchar message)
{
    switch (state)
    {
    case State::IDLE:
        break;

    case State::WAIT_UART:

        if (std::strcmp(message, "OEM") == 0)
        {
            HAL_USART1::SendString("\x40\x2A\x31\x0d");

            state = State::WAIT_MEASURE;
        }
        else if (std::strcmp(message, "USER") == 0)
        {
            HAL_USART1::SendString("\x40\x2A\x31\x0d");

            state = State::WAIT_MEASURE;
        }

        break;

    case State::WAIT_MEASURE:

        if (message[0] == '@' && std::strlen(message) == 3)
        {
            BitSet32 bs;
            bs.bytes[1] = (uint8)message[1];
            bs.bytes[0] = (uint8)message[2];

            concentrate.Set(Measure::ConcentrationCH4, (double)bs.half_word[0] * 1e-4);

            HAL_USART1::SetModeHC12();

            for (int counter = 0; counter < 3; counter++)
            {
                InterCom::Send(concentrate, TIME_MS);
            }

            HAL_USART1::SetModeSensor();
        }

        break;
    }
}
