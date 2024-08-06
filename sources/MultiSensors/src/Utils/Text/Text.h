// 2022/02/15 09:14:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"
#include "Utils/Text/String.h"


class Char
{
public:
    Char(char s) : symbol(s) {}
    int Draw(int x, int y, int size = 1, Color::E = Color::Count);
private:
    char symbol;
};


namespace Text
{
    // Возвращает высоту экрана, которую займёт текст text, при выводе от left до right в переменной height. Если
    // bool == false, то текст не влезет на экран 
    bool GetHeightTextWithTransfers(int left, int top, int right, pchar text, int *height);

    int DrawCharWithLimitation(int eX, int eY, uchar symbol, int limitX, int limitY, int limitWidth, int limitHeight);

    void Draw2Symbols(int x, int y, char symbol1, char symbol2, Color::E color1, Color::E color2);

    void Draw4SymbolsInRect(int x, int y, char eChar, Color::E = Color::Count);

    void Draw10SymbolsInRect(int x, int y, char eChar);

    String<> GetWord(pchar firstSymbol, int *length);

    // Если draw == false, то рисовать символ не надо, фунция используется только для вычислений
    int DrawPartWord(char *word, int x, int y, int xRight, bool draw);
}


namespace SU
{
    bool IsDigit(char symbol);

    String<> Bin2String(uint8 value);

    String<> Bin2String16(uint16 value);

    String<> FloatFract2String(float value, bool alwaysSign);

    String<> Float2String(float value, bool alwaysSign, int numDigits);

    String<> Hex8toString(uint8 value);

    String<> Int2String(int value, bool alwaysSign, int numMinFields);

    String<> Phase2String(float phase, bool alwaysSign);

    bool   String2Int(char *str, int *value);

    bool EqualsStrings(pchar str1, char *str2, int size);
    bool EqualsZeroStrings(char *str1, char *str2);

    // Возвращает количество слов в строке. Разделители - пробелы. Строка заканчивается байтами 0x0d, 0x0a.
    int GetNumWordsInString(pchar string);

    void LogBufferU8(const uint8 *data, int num);
    void LogBufferU8(pchar label, const uint8 *data, int num);
    void LogBufferF(pchar label, const float *data, int num);
};
