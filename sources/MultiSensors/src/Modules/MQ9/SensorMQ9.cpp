// 2025/03/13 08:02:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/MQ9/SensorMQ9.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/MQ9/TroykaMQ/MQ9.h"


namespace SensMQ9
{
    static bool is_init = false;

    bool IsInited()
    {
        return is_init;
    }

    static MQ9 mq9(0);
}


void SensMQ9::Init()
{
    HAL_ADC::Init();

    is_init = true;
}


bool SensMQ9::GetMeasure(Measure * measure)
{
    float monoxide = (float)mq9.readCarbonMonoxide();

    if (monoxide < 0.0f)
    {
        monoxide = 0.0f;
    }

    if (monoxide > 2000.0f)
    {
        monoxide = 2000.0f;
    }

    measure->Set(Measure::CarbonMonoxide, monoxide);

    return is_init;

    /*
    float voltage = HAL_ADC::GetVoltageDioxide();

    float RL = 1;

    float RS_gas = (3.3f - voltage) / voltage * RL;

    measure->Set(Measure::Dioxide, RS_gas);

    return is_init;
    */
}
