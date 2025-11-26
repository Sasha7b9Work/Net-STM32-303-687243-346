// 2025/11/26 10:58:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/MQ135/SensorMQ135.h"
#include "Hardware/HAL/HAL_PINS.h"


void SensorMQ135::Init()
{

}


bool SensorMQ135::IsInited()
{
    return false;
}


bool SensorMQ135::GetMeasure(Measure *)
{
    return false;
}


bool SensorMQ135::IsConnected()
{
    return pinB6.IsHi() && pinB7.IsLow();
}
