// 2022/03/12 09:37:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"


Color::E Color::current = Color::Count;


void Color::SetCurrent(Color::E color)
{
    if (color != Color::Count)
    {
        current = color;
    }
}


Color::E Color::GetCurrent()
{
    if (current == Color::FLASH_10)
    {
        return (Timer::CurrentTime() % 1000) < 500 ? Color::WHITE : Color::BLACK;
    }

    return current;
}


Color::E Color::MenuLetters(bool active)
{
    return active ? Color::WHITE : Color::GRAY_50;
}


Color::E Color::MenuItem()
{
    return Color::GREEN_50;
}
