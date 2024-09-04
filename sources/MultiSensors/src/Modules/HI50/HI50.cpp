// 2024/01/11 11:02:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/HI50/HI50.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/InterCom.h"
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

    static Measure distance;

    static bool is_exist = false;   // true, если модуль измерени€ дальности подключЄн
}


bool HI50::IsExist()
{
    return is_exist;
}


bool HI50::Init()
{
    HAL_USART1::SetModeHI50();

    state = State::WAIT_TURN_ON;

    HAL_USART1::Send(TURN_ON);

    TimeMeterMS meter;

    while (meter.ElapsedTime() < 500)
    {
        HAL_USART1::Update();

        if (state == State::WAIT_MEASURE)
        {
            is_exist = true;

            break;
        }
    }

    return is_exist;
}


void HI50::Update()
{
    if (!is_exist)
    {
        return;
    }

    switch (state)
    {
    case State::IDLE:
        HAL_USART1::Send(TURN_ON);
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
        HAL_USART1::Send(MEAS_HI);
        state = State::WAIT_MEASURE;
        break;

    case State::WAIT_MEASURE:

        // —юда попадает полное сообщение от измерител€

        static const int SIZE_BUFER = 128;

        char buffer_digits[SIZE_BUFER] = { '\0' };

        for (int i = 0; i < SIZE_BUFER; i++)
        {
            char buf[2] = { message[i], '\0' };
            std::strcat(buffer_digits, buf);

            if (buf[0] == 0)
            {
                if (buffer_digits[0] == 'M')
                {
                    for (int index = 3; index < SIZE_BUFER; index++)
                    {
                        if (buffer_digits[index] == 'm')
                        {
                            buffer_digits[index] = '\0';

                            std::memmove(buffer_digits, buffer_digits + 3, (uint)(index - 3));

                            float value = (float)std::atof(buffer_digits);

                            distance.Set(Measure::Distance, value);

//                            HAL_USART1::SetModeHC12();

                            Measure measure;

                            measure.Set(Measure::Distance, value);

//                            InterCom::Send(measure, TIME_MS);
                        }
                    }
                }
                else
                {
                    float value = 0.0f;

                    distance.Set(Measure::Distance, value);
//
//                    HAL_USART1::SetModeHC12();
//
//                    Measure measure;
//
//                    measure.Set(Measure::Distance, value);
//
//                    InterCom::Send(measure, TIME_MS);
//
//                    HAL_USART1::SetModeHI50();
                }

                HAL_USART1::Send(MEAS_HI);

                break;
            }
        }

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
