// 2022/03/30 08:25:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Font/Font.h"
#include "Utils/Text/String.h"
#include "Utils/Text/Text.h"
#include "Modules/ST7735/ST7735.h"
#include "Display/Display.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cstdlib>


template      String<(int)DEFAULT_SIZE_STRING>::String(pchar, ...);
template int  String<(int)DEFAULT_SIZE_STRING>::Draw(int, int, Color::E) const;
template void String<(int)DEFAULT_SIZE_STRING>::Append(pchar);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawInCenterRect(int x, int y, int width, int height, Color::E);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawWithLimitation(int x, int y, Color::E color, int limitX, int limitY, int limitWidth,
    int limitHeight);
template void String<(int)DEFAULT_SIZE_STRING>::DrawRelativelyRight(int xRight, int y, Color::E);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawOnBackground(int x, int y, Color::E colorBackground);
template void String<(int)DEFAULT_SIZE_STRING>::DrawInRect(int x, int y, int width, int height, int dy);
template void String<(int)DEFAULT_SIZE_STRING>::SetFormat(pchar format, ...);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawInBoundedRectWithTransfers(int x, int y, int width, Color::E colorBackground,
    Color::E colorFill);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawInRectWithTransfers(int x, int y, int width, int height, Color::E);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawStringInCenterRectAndBoundIt(int x, int y, int width, int height,
    Color::E colorBackground, Color::E colorFill);
template void String<(int)DEFAULT_SIZE_STRING>::Append(char);
template void String<(int)DEFAULT_SIZE_STRING>::DrawInCenterRectOnBackground(int x, int y, int width, int height, Color::E colorText,
    int widthBorder, Color::E colorBackground);
template bool String<(int)DEFAULT_SIZE_STRING>::ToInt(int *);
template void String<(int)DEFAULT_SIZE_STRING>::AppendBytes(const void *, int);

template int  String<(int)1024>::Draw(int, int, Color::E) const;
template int  String<(int)1024>::DrawInBoundedRectWithTransfers(int x, int y, int width, Color::E colorBackground,
    Color::E colorFill);


template<int capa>
int String<capa>::Draw(int x, int y, Color::E color) const
{
    Color::SetCurrent(color);

    if (Size() == 0)
    {
        return x;
    }

    pchar text = buffer;

    while (*text != '\0')
    {
        x = Char(*text).Draw(x, y) + 1;
        text++;
    }

    return x;
}


template<int capacity>
void String<capacity>::SetFormat(pchar format, ...)
{
    std::va_list args;
    va_start(args, format);
    int num_symbols = std::vsprintf(buffer, format, args);
    va_end(args);

    if(num_symbols < 0 || num_symbols > capacity - 1)
    {
        LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, num_symbols);
    }
}


template<int capacity>
String<capacity>::String(pchar format, ...)
{
    std::va_list args;
    va_start(args, format);
    int num_symbols = std::vsprintf(buffer, format, args);
    va_end(args);

    if(num_symbols < 0 || num_symbols > capacity - 1)
    {
        LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, num_symbols);
    }
}


template<int capacity>
void String<capacity>::Append(pchar str)
{
    int need_size = Size() + (int)std::strlen(str) + 1;

    if (need_size > capacity)
    {
        LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, need_size);

        int pointer = Size();

        while (pointer < capacity)
        {
            Append(str[pointer++]);
        }

        buffer[capacity - 1] = '\0';

        LOG_WRITE(buffer);
    }
    else
    {
        std::strcat(buffer, str);
    }
}


template<int capacity>
void String<capacity>::AppendBytes(const void *bytes, int num_bytes)
{
    int need_size = Size() + num_bytes + 1;

    if (need_size > capacity)
    {
        LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, need_size);
        LOG_WRITE(c_str());
    }
    else
    {
        std::memcpy(buffer + Size(), bytes, (uint)num_bytes);
        buffer[need_size - 1] = '\0';
    }
}


template<int capacity>
void String<capacity>::Append(char symbol)
{
    if (!Filled())
    {
        int pos = (int)std::strlen(buffer);
        buffer[pos] = symbol;
        buffer[pos + 1] = '\0';
    }
    else
    {
        LOG_ERROR_TRACE("buffer is full");
    }
}


template<int capacity>
int String<capacity>::DrawInCenterRect(int eX, int eY, int width, int eHeight, Color::E color)
{
    Color::SetCurrent(color);

    int lenght = Font::Text::GetLength(buffer);
    int height = Font::Symbol::Height(buffer[0]);
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2;

    return Draw(x, y);
}


template<int capacity>
int String<capacity>::DrawWithLimitation(int x, int y, Color::E color, int limitX, int limitY, int limitWidth, int limitHeight)
{
    Color::SetCurrent(color);

    uint result = (uint)x;

    char *text = buffer;

    while (*text)
    {
        x = Text::DrawCharWithLimitation(x, y, (uint8)*text, limitX, limitY, limitWidth, limitHeight);
        result += (uint)Font::Symbol::Width((uint8)*text);
        text++;
    }

    return (int)(result + 1);
}


template<int capacity>
void String<capacity>::DrawRelativelyRight(int xRight, int y, Color::E color)
{
    Color::SetCurrent(color);

    int lenght = Font::Text::GetLength(buffer);
    Draw(xRight - lenght, y);
}


template<int capacity>
int String<capacity>::DrawOnBackground(int x, int y, Color::E colorBackground)
{
    int width = Font::Text::GetLength(buffer);
    int height = Font::Height();

    Color::E colorText = Color::GetCurrent();
    Rectangle(width, height).Fill(x - 1, y, colorBackground);
    Color::SetCurrent(colorText);

    return Draw(x, y);
}


template<int capacity>
void String<capacity>::DrawInRect(int x, int y, int width, int, int dy)
{
    int xStart = x;
    int xEnd = xStart + width;

    char *text = buffer;

    while (*text != 0)
    {
        int length = GetLenghtSubString(text);

        if (length + x > xEnd)
        {
            x = xStart;
            y += Font::Symbol::Height(*text);
            y += dy;
        }

        int numSymbols = 0;
        numSymbols = DrawSubString(x, y, text);
        text += numSymbols;
        x += length;
        x = DrawSpaces(x, y, text, &numSymbols);
        text += numSymbols;
    }
}


template<int capacity>
int String<capacity>::GetLenghtSubString(char *text)
{
    int result = 0;

    while (((*text) != ' ') && ((*text) != '\0'))
    {
        result += Font::Symbol::Width((uint8)*text);
        text++;
        result++;
    }

    return result;
}


template<int capacity>
int String<capacity>::DrawSubString(int x, int y, char *text)
{
    int numSymbols = 0;

    while (((*text) != ' ') && ((*text) != '\0'))
    {
        x = Char(*text).Draw(x, y);
        numSymbols++;
        text++;
        x++;
    }

    return numSymbols;
}


template<int capacity>
int String<capacity>::DrawSpaces(int x, int y, char *text, int *numSymbols)
{
    *numSymbols = 0;

    while (*text == ' ')
    {
        x = Char(*text).Draw(x, y);
        text++;
        (*numSymbols)++;
    }

    return x;
}


template<int capacity>
int String<capacity>::DrawStringInCenterRectAndBoundIt(int x, int y, int width, int height, Color::E colorBackground,
    Color::E colorFill)
{
    Rectangle(width, height).Draw(x, y, colorFill);
    Rectangle(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    Color::SetCurrent(colorFill);
    return DrawInCenterRect(x, y, width, height);
}


template<int capacity>
int String<capacity>::DrawInBoundedRectWithTransfers(int x, int y, int width, Color::E colorBackground, Color::E colorFill)
{
    int height = 0;

    Text::GetHeightTextWithTransfers(x + 3, y + 3, x + width - 8, buffer, &height);

    Rectangle(width, height).Draw(x, y, colorFill);
    Rectangle(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    DrawInRectWithTransfers(x + 3, y + 3, width - 8, height, colorFill);
    return y + height;
}


template<int capacity>
int String<capacity>::DrawInRectWithTransfers(int eX, int eY, int eWidth, int eHeight, Color::E color)
{
    Color::SetCurrent(color);

    int top = eY;
    int left = eX;
    int right = eX + eWidth;
    int bottom = eY + eHeight;

    pchar text = buffer;

    int numSymb = (int)std::strlen(text);

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < bottom && curSymbol < numSymb)
    {
        while (x < right - 1 && curSymbol < numSymb)
        {
            int length = 0;
            String<> word = Text::GetWord(text + curSymbol, &length);

            if (length <= 1)                            // Нет буквенных символов или один, т.е. слово не найдено
            {
                char symbol = text[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                x = Char(symbol).Draw(x, y);
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::Text::GetLength(word.c_str());
                if (x + lengthString > right + 5)
                {
                    int numSymbols = Text::DrawPartWord(word.c_str(), x, y, right, true);
                    x = right;
                    curSymbol += numSymbols;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x = String<>(word).Draw(x, y);
                }
            }
        }
        x = left;
        y += 9;
    }

    return y;
}


template<int capacity>
void String<capacity>::DrawInCenterRectOnBackground(int x, int y, int width, int height, Color::E colorText,
    int widthBorder, Color::E colorBackground)
{
    int lenght = Font::Text::GetLength(buffer);
    int eX = DrawInCenterRect(x, y, width, height, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Rectangle(w, h).Fill(eX - lenght - widthBorder, y - widthBorder + 1);
    DrawInCenterRect(x, y, width, height, colorText);
}


template<int capacity>
bool String<capacity>::ToInt(int *out)
{
    char *str = buffer;

    int sign = str[0] == '-' ? -1 : 1;

    if (str[0] < '0' || str[0] > '9')
    {
        ++str;
    }

    int length = (int)(std::strlen(str));

    if (length == 0)
    {
        return false;
    }

    *out = 0;

    int pow = 1;

    for (int i = length - 1; i >= 0; i--)
    {
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *out += val * pow;
        pow *= 10;
    }

    if (sign == -1)
    {
        *out *= -1;
    }

    return true;
}
