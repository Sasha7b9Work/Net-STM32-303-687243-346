#pragma once
#include "defines.h"


#define TIME_MS Timer::CurrentTime()


namespace Timer
{
    uint CurrentTime();

    void Delay(uint delayMS);
}



// Структура для отсчёта времени
struct TimeMeterMS
{
    TimeMeterMS();

    // Установить момент отсчёта
    void Reset();

    // Столько миллисекунд прошло с момента вызова Reset()
    uint ElapsedTime();

    void PauseOnMS(uint);

    // Через ms миллисекунд IsFinished() становится равным true
    void FinishAfter(uint ms);

    bool IsFinished() const;

private:

    uint time_reset;        // От этого времени отсчитывается ElapsedTime()
    uint time_finished;     // В это время IsFinished() становится равным true
};
