// 2022/02/11 17:45:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Text/Text.h"
#include "Display/Font/Font.h"
#include "Modules/ST7735/ST7735.h"
#include "Utils/Math.h"
#include "Display/Display.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>



#define  SYMBOL(x) (*(*(x)))


namespace SU
{
    static bool ChooseSymbols(pchar *string);    // Возвращает false, если выбор невозможен - строка кончилась.
    static bool ChooseSpaces(pchar *string);     // Возвращает false, если выбор невозможен - строка кончилась.

    static int NumDigitsInIntPart(float value);
}


namespace Text
{
    int DrawCharWithLimitation(int eX, int eY, uchar symbol, int limitX, int limitY, int limitWidth, int limitHeight);

    // Возвращает высоту экрана, которую займёт текст text, при выводе от left до right в переменной height. Если
    // bool == false, то текст не влезет на экран 
    bool GetHeightTextWithTransfers(int left, int top, int right, pchar text, int *height);

    bool IsLetter(char symbol);

    bool IsConsonant(char symbol);

    // Находит следующий перенос. C letters начинается часть слово, где нужно найти перенос, в lettersInSyllable будет
    // записано число букв в найденном слоге. Если слово закончилось, функция возвращает false
    bool FindNextTransfer(char *letters, int *lettersInSyllable);

    int *BreakWord(char *word);

    bool CompareArrays(const bool *array1, const bool *array2, int numElems);

    // Возвращает часть слова до слога numSyllable(включительн) вместе со знаком переноса
    char *PartWordForTransfer(char *word, int *lengthSyllables, int numSyllables, int numSyllable, char buffer[30]);
}


int Char::Draw(int x, int y, int size, Color::E color)
{
    Color::SetCurrent(color);

    if (size == 1)
    {
        Font::Symbol::Draw(x, y, symbol);
    }
    else
    {
        char buffer[2] = { symbol, 0 };

        Font::Text::DrawBig(x, y, size, buffer);

        return x + size * Font::Symbol::Width((uint8)symbol);
    }

    return x + Font::Symbol::Width((uint8)symbol);
}


int Text::DrawCharWithLimitation(int eX, int eY, uchar symbol, int limitX, int limitY, int limitWidth, int limitHeight)
{
    int width = Font::Symbol::Width(symbol);
    int height = Font::Height();

    for (int b = 0; b < height; b++)
    {
        if (Font::Symbol::LineNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (Font::Symbol::BitInLineIsExist(symbol, b, bit))
                {
                    if ((x >= limitX) && (x <= (limitX + limitWidth)) && (y >= limitY) && (y <= limitY + limitHeight))
                    {
                        Point().Set(x, y);
                    }
                }
                x++;
            }
        }
    }

    return eX + width + 1;
}


bool Text::IsLetter(char symbol)
{
    static const bool isLetter[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true
    };

    return isLetter[(uint8)symbol];
}


String<> Text::GetWord(pchar firstSymbol, int *length)
{
    *length = 0;

    String<> result;

    while (IsLetter(*firstSymbol))
    {
        result.Append(*firstSymbol);
        firstSymbol++;
        (*length)++;
    }

    return result;
}


bool Text::IsConsonant(char symbol)
{
    static const bool isConsonat[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false
    };

    return isConsonat[(uint8)symbol];
}


bool Text::CompareArrays(const bool *array1, const bool *array2, int numElems)
{
    for (int i = 0; i < numElems; i++)
    {
        if (array1[i] != array2[i])
        {
            return false;
        }
    }
    return true;
}


bool Text::FindNextTransfer(char *letters, int *lettersInSyllable)
{

#define VOWEL       0   // Гласная
#define CONSONANT   1   // Согласная

    *lettersInSyllable = (int)std::strlen(letters);

    if (std::strlen(letters) <= 3) //-V1051
    {
        return false;
    }

    static const bool template1[3] = {false, true, true};               //     011     2   // После второго символа перенос
    static const bool template2[4] = {true, false, true, false};        //     1010    2
    static const bool template3[4] = {false, true, false, true};        //     0101    3
    static const bool template4[4] = {true, false, true, true};         //     1011    3
    static const bool template5[4] = {false, true, false, false};       //     0100    3
    static const bool template6[4] = {true, false, true, true};         //     1011    3
    static const bool template7[5] = {true, true, false, true, false};  //     11010   3
    static const bool template8[6] = {true, true, false, true, true};   //     11011   4

    bool consonant[20];

    int size = (int)std::strlen(letters);

    for (int i = 0; i < size; i++)
    {
        consonant[i] = IsConsonant(letters[i]);
    }

    if (CompareArrays(template1, consonant, 3))
    {
        *lettersInSyllable = 2;
        return true;
    }
    if (CompareArrays(template2, consonant, 4))
    {
        *lettersInSyllable = 2;
        return true;
    }
    if (std::strlen(letters) < 5)
    {
        return false;
    }
    if (CompareArrays(template3, consonant, 4) || CompareArrays(template4, consonant, 4) || CompareArrays(template5, consonant, 4) || CompareArrays(template6, consonant, 4))
    {
        *lettersInSyllable = 3;
        return true;
    }
    if (std::strlen(letters) < 6)
    {
        return false;
    }
    if (CompareArrays(template7, consonant, 5))
    {
        *lettersInSyllable = 3;
        return true;
    }
    if (CompareArrays(template8, consonant, 5))
    {
        *lettersInSyllable = 4;
        return true;
    }
    return false;
}


int *Text::BreakWord(char *word)
{
    int num = 0;
    static int lengthSyllables[10];
    char *position = word;

    while (FindNextTransfer(position, &(lengthSyllables[num])))
    {
        position += lengthSyllables[num];
        num++;
    }
    lengthSyllables[num + 1] = 0;
    if (std::strcmp(word, "структуру") == 0)
    {
        int8 lengths[] = {5, 2, 2, 0};
        std::memcpy(lengthSyllables, lengths, 4);
    }
    else if (std::strcmp(word, "соответствующей") == 0)
    {
        int8 lenghts[] = {4, 3, 4, 5, 3, 0};
        std::memcpy(lengthSyllables, lenghts, 6);
    }

    return lengthSyllables;
}


char* Text::PartWordForTransfer(char *word, int* lengthSyllables, int, int numSyllable, char buffer[30])
{
    int length = 0;

    for (int i = 0; i <= numSyllable; i++)
    {
        length += lengthSyllables[i];
    }

    std::memcpy((void*)buffer, (void*)word, (uint)length);
    buffer[length] = '-';
    buffer[length + 1] = '\0';
    return buffer;
}


int Text::DrawPartWord(char *word, int x, int y, int xRight, bool draw)
{
    int *lengthSyllables = BreakWord(word);
    int numSyllabels = 0;
    char buffer[30];

    for (int i = 0; i < 10; i++)
    {
        if (lengthSyllables[i] == 0)
        {
            numSyllabels = i;
            break;
        }
    }

    for (int i = numSyllabels - 2; i >= 0; i--)
    {
        char *subString = PartWordForTransfer(word, lengthSyllables, numSyllabels, i, buffer);
        int length = Font::Text::GetLength(subString);

        if (xRight - x > length - 5)
        {
            if (draw)
            {
                String<>(subString).Draw(x, y);
            }

            return (int)std::strlen(subString) - 1;
        }
    }

    return 0;
}


bool Text::GetHeightTextWithTransfers(int left, int top, int right, pchar text, int *height)
{
    int numSymb = (int)std::strlen(text);

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < 231 && curSymbol < numSymb)
    {
        while (x < right - 1 && curSymbol < numSymb)
        {
            int length = 0;
            String<> word = GetWord(text + curSymbol, &length);

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
                x += Font::Symbol::Width((uint8)symbol);
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::Text::GetLength(word.c_str());
                if (x + lengthString > right + 5)
                {
                    int numSymbols = DrawPartWord(word.c_str(), x, y, right, false);
                    x = right;
                    curSymbol += numSymbols;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x += Font::Text::GetLength(word.c_str());
                }
            }
        }
        x = left;
        y += 9;
    }

    LIMITATION(*height, y - top + 4, 0, 239)

    return curSymbol == numSymb;
}



void Text::Draw2Symbols(int x, int y, char symbol1, char symbol2, Color::E color1, Color::E color2)
{
    Char(symbol1).Draw(x, y, color1);
    Char(symbol2).Draw(x, y, color2);
}


void Text::Draw4SymbolsInRect(int x, int y, char eChar, Color::E color)
{
    Color::SetCurrent(color);

    for (int i = 0; i < 2; i++)
    {
        Char((char)(eChar + i)).Draw(x + 8 * i, y);
        Char((char)(eChar + i + 16)).Draw(x + 8 * i, y + 8);
    }
}


void Text::Draw10SymbolsInRect(int x, int y, char eChar)
{
    for (int i = 0; i < 5; i++)
    {
        Char((char)(eChar + i)).Draw(x + 8 * i, y);
        Char((char)(eChar + i + 16)).Draw(x + 8 * i, y + 8);
    }
}


static int SU::NumDigitsInIntPart(float value)
{
    float fabsValue = std::fabs(value);

    int numDigitsInInt = 0;

    if (fabsValue >= 10000)
    {
        numDigitsInInt = 5;
    }
    else if (fabsValue >= 1000)
    {
        numDigitsInInt = 4;
    }
    else if (fabsValue >= 100)
    {
        numDigitsInInt = 3;
    }
    else if (fabsValue >= 10)
    {
        numDigitsInInt = 2;
    }
    else
    {
        numDigitsInInt = 1;
    }

    return numDigitsInInt;
}


int SU::GetNumWordsInString(pchar string)
{
    SU::ChooseSpaces(&string);

    while (true)
    {
        int numWords = 0;

        if (SU::ChooseSymbols(&string))
        {
            numWords++;
        }
        else
        {
            return numWords;
        }
        SU::ChooseSpaces(&string);
    }
}


bool SU::ChooseSymbols(pchar *string)
{
    if (SYMBOL(string) == 0)
    {
        return false;
    }

    while (SYMBOL(string) != ' ' && SYMBOL(string) != 0)
    {
        (*string)++;
    }

    return true;
}


bool SU::ChooseSpaces(pchar *string)
{
    if (SYMBOL(string) == 0)
    {
        return false;
    }

    while (SYMBOL(string) == ' ')
    {
        (*string)++;
    }

    return true;
}


bool SU::EqualsStrings(pchar str1, char *str2, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}


bool SU::EqualsZeroStrings(char *str1, char *str2)
{
    while ((*str1) == (*str2))
    {
        if ((*str1) == '\0')
        {
            return true;
        }
        str1++;
        str2++;
    }
    return false;
}


void SU::LogBufferU8(const uint8 *data, int num)
{
    String<> result;

    for (int i = 0; i < num; i++)
    {
        result.Append(SU::Int2String(data[i], false, 1));
        result.Append(" ");
    }

    LOG_WRITE(result.c_str());
}


void SU::LogBufferU8(pchar label, const uint8 *data, int num)
{
    String<> result(label);
    result.Append(" ");

    for (int i = 0; i < num; i++)
    {
        result.Append(SU::Int2String(data[i], false, 1));
        result.Append(" ");
    }

    LOG_WRITE(result.c_str());
}


void SU::LogBufferF(pchar label, const float *data, int num)
{
    String<> string(label);
    string.Append(" ");

    for (int i = 0; i < num; i++)
    {
        string.Append(SU::Float2String(data[i], false, 4));
        string.Append(" ");
    }

    LOG_WRITE(string.c_str());
}


String<> SU::FloatFract2String(float value, bool alwaysSign)
{
    return String<>(Float2String(value, alwaysSign, 4));
}


String<> SU::Bin2String(uint8 value)
{
    char buffer[9];

    for (int bit = 0; bit < 8; bit++)
    {
        buffer[7 - bit] = _GET_BIT(value, bit) ? '1' : '0';
    }

    return String<>(buffer);
}


String<> SU::Bin2String16(uint16 value)
{ 
    char buffer[19];

    std::strcpy(buffer, Bin2String((uint8)(value >> 8)).c_str());
    buffer[8] = ' ';
    std::strcpy(buffer + 9, Bin2String((uint8)value).c_str());

    return String<>(buffer);
}


String<> SU::Float2String(float value, bool alwaysSign, int numDigits)
{
    String<> result;

    char *buffer = result.c_str();

    if (value == ERROR_VALUE_FLOAT || isnan(value) || isinf(value)) //-V550
    {
        return EmptyString();
    }

    if (!alwaysSign)
    {
        if (value < 0)
        {
            *buffer = '-';
            buffer++;
        }
    }
    else
    {
        *buffer = value < 0 ? '-' : '+';
        buffer++;
    }

    char format[] = "%4.2f\0\0";

    format[1] = (char)numDigits + 0x30;

    int numDigitsInInt = NumDigitsInIntPart(value);

    format[3] = (char)((numDigits - numDigitsInInt) + 0x30);
    if (numDigits == numDigitsInInt)
    {
        format[5] = '.';
    }

    std::snprintf(buffer, 19, (const char *)&format[0], (double)std::fabsf(value));
    
    if((int)std::strlen(buffer) > numDigits + 1)        // Это затычка на то, что число -9.99999 выводится как -10.000, хотя должно быть четыре цифры - 2 цифры после запятой
    {
        buffer[numDigits + 1] = '\0';
    }

    float val = (float)std::atof(buffer);

    if (NumDigitsInIntPart(val) != numDigitsInInt)
    {
        numDigitsInInt = NumDigitsInIntPart(val);
        format[3] = (char)((numDigits - numDigitsInInt) + 0x30);

        if (numDigits == numDigitsInInt)
        {
            format[5] = '.';
        }
    }

    bool signExist = alwaysSign || value < 0;

    while (result.Size() < numDigits + (signExist ? 2 : 1))
    {
        result.Append("0");
    }

    if (result.Size() > numDigits + 2)
    {
        LOG_ERROR("Неправильное значение: %s", result.c_str());
    }

    return result;
}


String<> SU::Int2String(int value, bool alwaysSign, int numMinFields)
{
    char buffer[20];

    char format[20] = "%";
    std::sprintf(&(format[1]), "0%d", numMinFields);
    std::strcat(format, "d");

    if (alwaysSign && value >= 0)
    {
        buffer[0] = '+';
        std::sprintf(buffer + 1, format, value);
    }
    else
    {
        std::sprintf(buffer, format, value);
    }

    return String<>(buffer);
}


String<> SU::Hex8toString(uint8 value)
{
    char buffer[3];

    if(value < 16)
    {
        buffer[0] = '\0';
        std::sprintf(buffer + 1, "%x", value);
    }
    else
    {
        std::sprintf(buffer, "%x", value);
    }
    
    if(value < 16)
    {
        buffer[0] = '0';
        std::sprintf(buffer + 1, "%x", value);
    }
    else
    {
        std::sprintf(buffer, "%x", value);
    }
    return String<>(buffer);
}


bool SU::String2Int(char *str, int *value)
{
    int sign = str[0] == '-' ? -1 : 1;

    if (str[0] < '0' || str[0] > '9')
    {
        str++;
    }

    int length = (int)std::strlen(str);

    if (length == 0)
    {
        return false;
    }

    *value = 0;
    int pow = 1;

    for (int i = length - 1; i >= 0; i--)
    {
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    }

    if (sign == -1)
    {
        *value *= -1;
    }

    return true;
}


String<> SU::Phase2String(float phase, bool)
{
    String<> result = SU::Float2String(phase, false, 4);
    result.Append("\xa8");
    return result;
}


bool SU::IsDigit(char symbol)
{
    return (symbol >= '0') && (symbol <= '9');
}


#undef SYMBOL
