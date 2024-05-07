// 2024/05/06 15:43:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/L00256L/L00256L.h"
#include "Hardware/Timer.h"
#include <stm32f3xx_hal.h>


namespace L00256L
{
    TimeMeterMS meter;

    bool pressed = false;               // Если true, клавиша нажата
    bool taboo_long = false;            // Если true, запрещено длинное срабатывание

#define STATE_A (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
#define STATE_B (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)

    static bool prev_a = false;

    static const float step_angle = 360.0f / 256.0f;
    static float angle_full = 0;

    static bool is_enabled = false;

    namespace Velocity
    {
        static uint time_start = 0;
        static float angle = 0.0f;
        static float speed = 0.0f;

        static void CallbackOnInterrupt(float delta)
        {
            angle += delta;
        }

        static void Update()
        {
            static uint delta_time = 256;

            if (time_start == 0)
            {
                time_start = TIME_MS;
            }

            if (TIME_MS - time_start >= delta_time)
            {
                speed = angle / (float)(TIME_MS - time_start) * 1e3f;

                angle = 0;
                time_start = TIME_MS;
            }
        }
    }
}


void L00256L::Init()
{
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_0,
        GPIO_MODE_IT_RISING_FALLING,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_HIGH,
        0
    };

    HAL_GPIO_Init(GPIOA, &is);

    is.Pin = GPIO_PIN_1;
    is.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &is);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    is_enabled = true;
}


bool L00256L::IsEnabled()
{
    return is_enabled;
}


void L00256L::CallbackOnInterrupt()
{
    static bool first = true;

    float before = angle_full;

    if (first)
    {
        first = false;

        prev_a = STATE_A;

        return;
    }

    bool state_a = STATE_A;

    if (state_a && !prev_a)
    {
        if (STATE_B)
        {
            angle_full -= step_angle;
        }
        else
        {
            angle_full += step_angle;
        }
    }

    prev_a = state_a;

    Velocity::CallbackOnInterrupt(angle_full - before);
}


void L00256L::Update()
{
    Velocity::Update();
}


float L00256L::GetAngleFull()
{
    return (float)angle_full;
}


float L00256L::GetAngleRelative()
{
    float angle = angle_full;

    while (angle < 0.0f)
    {
        angle += 360.0f;
    }

    while (angle >= 360.0f)
    {
        angle -= 360.0f;
    }

    return angle;
}


float L00256L::GetVelocity()
{
    return Velocity::speed;
}
