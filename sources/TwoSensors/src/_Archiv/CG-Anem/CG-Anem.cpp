// 2022/02/14 11:59:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CG-Anem/CG-Anem.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f3xx_hal.h>
#include <cstdlib>
#include <cstring>


namespace CG_Anem
{
    static unsigned int timeNext = 1;
}


bool CG_Anem::GetMeasure(Measure *velocity_out)
{
    velocity_out->Clear();

    if (HAL_GetTick() < timeNext)
    {
        return false;
    }

    timeNext += TIME_MEASURE + (uint)(std::rand() % 100);

#ifdef IN_MODE_TEST

    velocity_out->Set(Measure::Velocity, (float)(std::rand() % 10000) / 100.0f);

    return true;

#else

    bool result = true;

    BitSet32 temp_cold;

    if (HAL_I2C1::Read(0x11, 0x10, &temp_cold.bytes[1], 1) != 0)
    {
        result = false;
    }

    if (HAL_I2C1::Read(0x11, 0x11, &temp_cold.bytes[0], 1) != 0)
    {
        result = false;
    }

    BitSet32 velocity;
    
    if (HAL_I2C1::Read(0x11, 0x07, &velocity.bytes[1], 1) != 0)
    {
        result = false;
    }

    if (HAL_I2C1::Read(0x11, 0x08, &velocity.bytes[0], 1) != 0)
    {
        result = false;
    }

    if (result)
    {
        velocity_out->Set(Measure::Velocity, velocity.half_word[0] * 0.1f);
    }

    return result;

#endif
}
