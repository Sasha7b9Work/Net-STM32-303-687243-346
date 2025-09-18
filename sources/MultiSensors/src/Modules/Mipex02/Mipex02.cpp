// 2025/09/18 08:51:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/Mipex02/Mipex02.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"


namespace Mipex02
{
    static bool is_exist = false;

    struct State
    {
        enum E
        {
            IDLE,
            WAIT_UART,          // Послали команду UART?, ждём ответа
            WAIT_MEASURE        // Послана команда запроса измерения, ждём его
        };
    };

    static State::E state = State::IDLE;
}


bool Mipex02::Init()
{
    is_exist = false;
    
    HAL_USART1::SetModeSensor();

    state = State::WAIT_UART;

    HAL_USART1::SendString("UART\0x0d");

    TimeMeterMS meter;

    while (meter.ElapsedTime() < 1000)
    {
        HAL_USART1::Update();

        if (state == State::WAIT_MEASURE)
        {
            is_exist = true;

            break;
        }
    }

    return IsExist();
}


bool Mipex02::IsExist()
{
    return is_exist;
}


void Mipex02::Update()
{

}


bool Mipex02::GetMeasure(Measure *)
{
    return false;
}


void Mipex02::CallbackOnReceive(pchar message)
{
    switch (state)
    {
    case State::IDLE:
        break;

    case State::WAIT_UART:

        message = message;

        HAL_USART1::SendString("@*1\0x0d");

        state = State::WAIT_MEASURE;

        break;

    case State::WAIT_MEASURE:
        break;
    }
}
