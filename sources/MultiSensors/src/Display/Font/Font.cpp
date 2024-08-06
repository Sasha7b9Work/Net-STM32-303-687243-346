// 2022/02/11 15:58:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Font.h"
#include "Display/Display.h"

#include "font8.inc"
#include "font12_10.inc"



namespace Font
{
    static TypeFont::E current = TypeFont::_8;
}


int Font::Text::GetLength(pchar text, int size)
{
    uint result = 0;

    while (*text)
    {
        result += (uint)Font::Symbol::Width((uint8)*text) * (uint)size + (uint)Font::GetSpacing() * (uint)size;
        text++;
    }

    return (int)result;
}


int Font::Symbol::Height(char)
{
    return 9;
}


int Font::GetSpacing()
{
    return 1;
}


void Font::Set(TypeFont::E typeFont)
{
    if (typeFont == current)
    {
        return;
    }

    current = typeFont;
}


int Font::Symbol::Width(uint8 symbol)
{
    int result = 0;

    switch (current)
    {
    case TypeFont::_8:      result = (symbol == ' ') ? 5 : font8.symbol[symbol].width;       break;
    case TypeFont::_12_10:  result = (symbol == ' ') ? 8 : font12_10.symbol[symbol].width;   break;
    case TypeFont::Count:                                                                    break;
    }

    return result;
}


int Font::Height()
{
    int result = 0;

    switch (current)
    {
    case TypeFont::_8:      result = font8.height;        break;
    case TypeFont::_12_10:  result = font12_10.height;    break;
    case TypeFont::Count:   break;
    }

    return result;
}


int Font::Width()
{
    int result = 0;

    switch (current)
    {
    case TypeFont::_8:      result = 8;                 break;
    case TypeFont::_12_10:  result = font12_10.width;   break;
    case TypeFont::Count:   break;
    }

    return result;
}


bool Font::Symbol::LineNotEmpty(uint eChar, int line)
{
    if (current == TypeFont::_8)
    {
        static const uint8 *bytes = 0;
        static uint prevChar = (uint)(-1);

        if (eChar != prevChar)
        {
            prevChar = eChar;
            bytes = font8.symbol[prevChar].bytes;
        }

        return bytes[line] != 0;
    }
    else if (current == TypeFont::_12_10)
    {
        return font12_10.symbol[eChar].words[line] != 0;
    }

    return false;
}


bool Font::Symbol::BitInLineIsExist(uint eChar, int line, int bit)
{
    if (current == TypeFont::_8)
    {
        static uint8 prevByte = 0;      // WARN здесь точно статики нужны?
        static uint prevChar = (uint)(-1);
        static int prevNumByte = -1;

        if (prevNumByte != line || prevChar != eChar)
        {
            prevByte = font8.symbol[eChar].bytes[line];
            prevChar = eChar;
            prevNumByte = line;
        }

        return prevByte & (1 << bit);
    }
    else if (current == TypeFont::_12_10)
    {
        return (font12_10.symbol[eChar].words[line] & (1 << (9 - bit))) != 0;
    }

    return false;
}


int Font::Symbol::Draw(int eX, int eY, char s)
{
    uint8 symbol = (uint8)s;

    int width = Font::Symbol::Width(symbol);
    int height = Font::Height();
    int width_font = Font::Width();

    for (int line = 0; line < height; line++)
    {
        if (Font::Symbol::LineNotEmpty(symbol, line))
        {
            int x = eX;
            int endBit = width_font - width;

            for (int bit = width_font - 1; bit >= endBit; bit--)
            {
                if (Font::Symbol::BitInLineIsExist(symbol, line, bit))
                {
                    Point().Set(x, eY);
                }
                x++;
            }
        }

        eY++;
    }

    return eX + width;
}


int Font::Symbol::DrawBig(int eX, int eY, int size, char s)
{
    eY++;

    uint8 symbol = (uint8)s;

    int width = Font::Symbol::Width(symbol);
    int height = Font::Height();
    int width_font = Font::Width();

    for (int b = 0; b < height; b++)
    {
        if (Font::Symbol::LineNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + width_font + 1 - height;
            int endBit = width_font - width;
            for (int bit = width_font - 1; bit >= endBit; bit--)
            {
                if (Font::Symbol::BitInLineIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            Point().Set(x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}


void Font::Text::DrawBig(int eX, int eY, int size, pchar buffer, Color::E color)
{
    Color::SetCurrent(color);

    int numSymbols = (int)std::strlen(buffer);

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = Font::Symbol::DrawBig(x, eY, size, buffer[i]);
        x += size;
    }
}
