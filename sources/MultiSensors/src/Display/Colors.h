// 2022/03/12 09:37:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Color
{
    enum E
    {
        WHITE,      // 0
        BLACK,      // 1
        RED,        // 2
        GREEN,      // 3
        _1,         // 4
        GREEN_50,   // 5
        BLUE,       // 6
        GRAY_50,    // 7
        GRAY_25,    // 8
        GRAY_10,    // 9
        FLASH_10,
        Count
    };

    E value;

    Color(E v) : value(v) {}

    static void SetCurrent(E);

    static E GetCurrent();

    static const uint16 colors[Count];

    // Цвет надписей
    static Color::E MenuLetters(bool active);
    static Color::E MenuItem();

private:

    static Color::E current;
};


#define MAKE_COLOR(r, g, b)  (uint16)((b) + ((g) << 5) + ((r) << 11))

#define BLUE_FROM_COLOR(color)  ((color) & 0x1f)
#define GREEN_FROM_COLOR(color) (((color) >> 5) & 0x3f)
#define RED_FROM_COLOR(color)   (((color) >> 11) & 0x1f)
