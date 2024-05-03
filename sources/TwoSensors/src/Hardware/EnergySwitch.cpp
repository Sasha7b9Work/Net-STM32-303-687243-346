// 2023/11/09 14:19:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/EnergySwitch.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"


void EnergySwitch::Init()
{
    pinPowerIn.Init();

    pinPowerOut.Init();

    pinPowerOut.ToHi();

    pinOUT.Init();
}


void EnergySwitch::Update()
{
#ifndef WIN32

    if (TIME_MS < 1000)
    {
        return;
    }

    if (pinPowerIn.IsHi())
    {
        Timer::Delay(1000);

        pinPowerOut.ToLow();
    }

    if (pinOUT.IsHi())
    {
        pinOUT.ToLow();
    }
    else
    {
        pinOUT.ToHi();
    }

#endif
}
