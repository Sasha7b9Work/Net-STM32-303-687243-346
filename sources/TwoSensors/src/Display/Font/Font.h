// 2022/02/11 15:58:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


struct TypeFont
{
    enum E
    {
        _8,
        _12_10,
        Count
    };
};


namespace Font
{
    int GetSpacing();
    int Height();
    int Width();
    void Set(TypeFont::E);

    namespace Symbol
    {
        int  Width(uint8);
        int  Height(char symbol);
        bool LineNotEmpty(uint symbol, int line);
        bool BitInLineIsExist(uint symbol, int line, int bit);

        int Draw(int x, int y, char symbol);
        int DrawBig(int eX, int eY, int size, char s);
    }

    namespace Text
    {
        int GetLength(pchar, int size = 1);
        void DrawBig(int eX, int eY, int size, pchar, Color::E = Color::Count);
    }
};


struct Symbol8
{
    uchar width;
    uchar bytes[8];
};


struct Font8
{
    int height;
    Symbol8 symbol[256];
};


struct Symbol22
{
    uint8 width;
    int16 words[12];
};


// Ўрифт макимальными размерам 2х2 байта
struct Font22
{
    int height;
    int width;

    Symbol22 symbol[256];
};
