// 2025/11/26 10:58:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/MQ135/SensorMQ135.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/MQ135/MQ135.h"


namespace SensorMQ135
{
    static bool is_init = false;

    bool IsInited()
    {
        return is_init;
    }

    static MQ135 mq135;
}


void SensorMQ135::Init()
{
    HAL_ADC::Init();

    is_init = true;
}


bool SensorMQ135::GetMeasure(Measure *measure)
{
    float dioxide = mq135.getPPM() / 90.0f;

    if (dioxide < 0.0f)
    {
        dioxide = 0.0f;
    }

    measure->Set(Measure::CarbonDioxide, dioxide);

    return is_init;
}


bool SensorMQ135::IsConnected()
{
    return pinB6.IsHi() && pinB7.IsLow();
}
