// 2026/03/03 12:14:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"


struct Record
{
    uint       crc;
    uint       number;      // Порядковый номер записи. Нужно для нахождения последней и первой
    PackedTime time;
    Measure    measure;

    uint CalculateCRC() const;
    bool IsValid() const;
    int Size() const;
    uint8 *Begin() const
    {
        return (uint8 *)this;
    }
    // Начало (за исключением crc)
    uint8 *BeginData() const
    {
        return Begin() + sizeof(crc);
    }
    // Размер данных (за исключением crc)
    int SizeData() const
    {
        return sizeof(*this) - sizeof(crc);
    }
};
