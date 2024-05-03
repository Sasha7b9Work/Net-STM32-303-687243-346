// 2022/03/30 08:25:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"
#include <cstring>


#define DEFAULT_SIZE_STRING 64

template<int capacity = DEFAULT_SIZE_STRING>
class String
{
public:

    String() { buffer[0] = '\0'; }

    explicit String(pchar, ...);

    void SetFormat(pchar format, ...);
        
    char *c_str() const { return (char *)&buffer[0]; }

    int Size() const { return (int)std::strlen(buffer); }

    void Append(pchar);

    void Append(char);

    void Append(const String<capacity> &rhs) { Append(rhs.c_str()); }

    // Добавить байты. В конец дописать ноль
    void AppendBytes(const void *bytes, int num_bytes);

    bool Filled() const { return (Size() == capacity - 1); }

    int Draw(int x, int y, Color::E = Color::Count) const;

    void DrawInRect(int x, int y, int width, int height, int dy = 0);

    int DrawInCenterRect(int x, int y, int width, int height, Color::E = Color::Count);

    int DrawWithLimitation(int x, int y, Color::E color, int limitX, int limitY, int limitWidth, int limitHeight);

    void DrawRelativelyRight(int xRight, int y, Color::E = Color::Count);

    // Выводит текст на прямоугольнике цвета colorBackgound
    int DrawOnBackground(int x, int y, Color::E colorBackground);

    int DrawStringInCenterRectAndBoundIt(int x, int y, int width, int height, Color::E colorBackground, Color::E colorFill);

    // Возвращает нижнюю координату прямоугольника.
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color::E colorBackground, Color::E colorFill);

    int DrawInRectWithTransfers(int x, int y, int width, int height, Color::E = Color::Count);

    // Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра
    // widthBorder цвета colorBackground.
    void DrawInCenterRectOnBackground(int x, int y, int width, int height, Color::E colorText, int widthBorder,
        Color::E colorBackground);

    char &operator[](int i)
    {
        if (i >= 0 && i < Size())
        {
            return buffer[i];
        }

        static char null_char = 0;

        return null_char;
    }

    bool ToInt(int *out);

private:

    char buffer[capacity];

    int GetLenghtSubString(char *text);

    int DrawSubString(int x, int y, char *text);

    int DrawSpaces(int x, int y, char *text, int *numSymbols);
};


class EmptyString : public String<>
{
public:
    EmptyString() : String<>("--.--") { }
};
