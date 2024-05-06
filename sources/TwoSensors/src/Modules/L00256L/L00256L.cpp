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
}


void L00256L::Init()
{
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_0,
        GPIO_MODE_INPUT,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_HIGH,
        0
    };

    HAL_GPIO_Init(GPIOA, &is);

    is.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &is);
}


void L00256L::Update()
{
    static bool first = true;

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
