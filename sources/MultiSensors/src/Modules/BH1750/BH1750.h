// 2022/02/21 15:39:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Measures.h"

/*
* Датчик освещённости
*/

namespace BH1750
{
    bool Init();

    bool GetMeasure(Measure *illumination);
}
