// 2022/05/06 11:30:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include <stm32f3xx_hal.h>


Key key1(Key::_1);
Key key2(Key::_2);


namespace Keyboard
{
    static const int TIME_LONG_PRESS = 500;

    struct KeyStruct
    {
        bool pressed;           // Если true, клавиша нажата
        bool taboo_long;        // Если true, запрещено длинное срабатывание
        TimeMeterMS meter;
    };

    static KeyStruct keys[Key::Count] =
    {
        { false, false },
        { false, false }
    };

    static bool KeyPressed(const Key &);
    static void UpdateKey(const Key &);

    namespace IT
    {
        static bool pressed[Key::Count] = { false, false };
    }
}


void Keyboard::Init()
{
    /*
    * PB8, PB9
    */

    pinKey1.Init();
    pinKey2.Init();

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}


void Keyboard::Update()
{
    for (int i = 0; i < Key::Count; i++)
    {
        UpdateKey((Key::E)i);
    }
}


void Keyboard::UpdateKey(const Key &k)
{
    KeyStruct &key = keys[k.value];

    if (key.pressed)
    {
        if (key.meter.ElapsedTime() > TIME_LONG_PRESS && !key.taboo_long)
        {
            Menu::LongPress(k);
            key.taboo_long = true;
        }
        else
        {
            if (!KeyPressed(k))
            {
                key.pressed = false;
                key.meter.Reset();
                if (!key.taboo_long)
                {
                    Menu::ShortPress(k);
                }
                key.taboo_long = false;
            }
        }
    }
    else
    {
        if (KeyPressed(k))
        {
            key.pressed = true;
            key.meter.Reset();
        }
    }
}


bool Keyboard::KeyPressed(const Key &key)
{
    return IT::pressed[key.value];
}


bool Key::IsPressed() const
{
    return Keyboard::keys[value].pressed;
}


void HAL_GPIO_EXTI_Callback(uint16_t)
{
    Keyboard::IT::pressed[Key::_1] = pinKey1.IsLow();

    Keyboard::IT::pressed[Key::_2] = pinKey2.IsLow();
}


void EXTI9_5_IRQHandler(void)
{
    if (GPIOB->IDR & GPIO_PIN_8)                                // Не нажата
    {
        if (Keyboard::IT::pressed[Key::_1])
        {
            Keyboard::IT::pressed[Key::_1] = false;

            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
        }
    }
    else                                                        // Нажата
    {
        if (!Keyboard::IT::pressed[Key::_1])
        {
            Keyboard::IT::pressed[Key::_1] = true;

            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
        }
    }

    if(GPIOB->IDR & GPIO_PIN_9)
    {
        if (Keyboard::IT::pressed[Key::_2])
        {
            Keyboard::IT::pressed[Key::_2] = false;

            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
        }
    }
    else
    {
        if (!Keyboard::IT::pressed[Key::_2])
        {
            Keyboard::IT::pressed[Key::_2] = true;
        }

        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
    }
}
