#include "defines.h"
#include "Hardware/Timer.h"
#include <stm32f3xx_hal.h>


uint Timer::CurrentTime()
{
    return HAL_GetTick();
}


void Timer::Delay(uint delayMS)
{
    uint end = TIME_MS + delayMS;

    while (TIME_MS < end)
    {
    }
}


TimeMeterMS::TimeMeterMS()
{
    Reset();
}


void TimeMeterMS::Reset()
{
    time_reset = TIME_MS;
    time_finished = time_reset;
}


uint TimeMeterMS::ElapsedTime()
{
    return TIME_MS - time_reset;
}


void TimeMeterMS::FinishAfter(uint timeMS)
{
    time_finished = TIME_MS + timeMS;
}


bool TimeMeterMS::IsFinished() const
{
    return TIME_MS >= time_finished;
}


void TimeMeterMS::PauseOnMS(uint dT)
{
    uint time_enter = ElapsedTime();

    while (ElapsedTime() - time_enter < dT)
    {

    }
}
