// 2025/03/13 08:02:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/MQ9/MQ9.h"
#include "Hardware/HAL/HAL.h"


namespace MQ9
{
    static bool is_init = false;

    bool IsInited()
    {
        return is_init;
    }
}


void MQ9::Init()
{
    HAL_ADC::Init();

    is_init = true;
}


bool MQ9::GetMeasure(Measure * measure)
{
    measure->Set(Measure::Dioxide, HAL_ADC::GetVoltageDioxide());

    return is_init;
}
