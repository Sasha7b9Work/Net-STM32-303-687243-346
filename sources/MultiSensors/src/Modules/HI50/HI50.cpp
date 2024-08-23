// 2024/01/11 11:02:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/HI50/HI50.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>


namespace HI50
{
    static const uint8 TURN_ON = 0x4f;
//    static const uint8 MEAS_AUTO = 0x44;
    static const uint8 MEAS_HI = 0x4D;

    struct State
    {
        enum E
        {
            IDLE,
            WAIT_TURN_ON,
            WAIT_MEASURE
        };
    };

    static State::E state = State::IDLE;

    static bool is_init = false;

    static Measure distance;

    static void CallbackOnReceive(pchar);
}


bool HI50::Init()
{
    HAL_USART_HI50::Init(CallbackOnReceive);

    state = State::WAIT_TURN_ON;

    HAL_USART_HI50::Send(TURN_ON);

    TimeMeterMS meter;

    while (meter.ElapsedTime() < 500)
    {
        HAL_USART_HI50::Update();

        if (state == State::WAIT_MEASURE)
        {
            break;
        }
    }

    is_init = (state == State::WAIT_MEASURE);

    return is_init;
}


void HI50::Update()
{
    if (!is_init)
    {
        return;
    }

    switch (state)
    {
    case State::IDLE:
        HAL_USART_HI50::Send(TURN_ON);
        state = State::WAIT_TURN_ON;
        break;

    case State::WAIT_TURN_ON:
        break;

    case State::WAIT_MEASURE:
        break;
    }
}


void HI50::CallbackOnReceive(pchar message)
{
    switch (state)
    {
    case State::IDLE:
        break;

    case State::WAIT_TURN_ON:
        HAL_USART_HI50::Send(MEAS_HI);
        state = State::WAIT_MEASURE;
        break;

    case State::WAIT_MEASURE:

        char buffer_digits[32] = { '\0' };

        int index = 3;

        while (message[index] != '.' && message[index] != '!')
        {
            char buf[2] = { '\0', '\0' };
            buf[0] = message[index++];
            std::strcat(buffer_digits, buf);

            if (std::strlen(buffer_digits) > 30)
            {
                index = index;
            }
        }

        float integer = (float)std::atoi(buffer_digits);

        index++;
        buffer_digits[0] = { '\0' };
        while (message[index] != 'm')
        {
            char buf[2] = { message[index++], '\0' };
            std::strcat(buffer_digits, buf);
        }

        float fract = (float)std::atoi(buffer_digits);

        for (uint i = 0; i < std::strlen(buffer_digits); i++)
        {
            fract /= 10.0f;
        }

        distance.Set(Measure::Distance, integer + fract);

        // \todo
        /*
        * «десь нужно распарсить полученое сообщение
        */
        HAL_USART_HI50::Send(MEAS_HI);
        break;
    }
}


bool HI50::GetMeasure(Measure *measure)
{
    if (distance.correct)
    {
        *measure = distance;

        return true;
    }

    return false;
}
