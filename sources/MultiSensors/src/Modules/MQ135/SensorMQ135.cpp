// 2025/11/26 10:58:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/MQ135/SensorMQ135.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/HAL/HAL.h"


namespace SensorMQ135
{
    static bool is_init = false;

    bool IsInited()
    {
        return is_init;
    }
}


void SensorMQ135::Init()
{
    HAL_ADC::Init();

    is_init = true;
}


bool SensorMQ135::GetMeasure(Measure *)
{
    return false;
}


bool SensorMQ135::IsConnected()
{
    return pinB6.IsHi() && pinB7.IsLow();
}
