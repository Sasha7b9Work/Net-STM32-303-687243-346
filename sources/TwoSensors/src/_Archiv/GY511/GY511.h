// 2023/01/30 16:44:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Measures.h"


// Структура содержит нативные данные, считанные из акселерометра
union StructDataRaw
{
    union
    {
        uint8 byte[2];
        int16 raw;
    };

    float ToAccelerate()
    {
        return (float)raw / 16.0f / 1000.0f;
    }
};


namespace GY511
{
    void Init();

    bool GetAzimuth(Measure *azimuth);

    float GetX();

    float GetY();

    float GetZ();
}
