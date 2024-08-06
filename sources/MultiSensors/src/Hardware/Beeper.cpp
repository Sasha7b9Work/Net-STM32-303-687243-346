// 2022/6/19 6:32:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Storage/Measures.h"
#include <stm32f3xx_hal.h>


namespace Beeper
{
    static TIM_HandleTypeDef handle =
    {
        TIM3,
        {
            (uint)(60000 / 2000 - 1),
            TIM_COUNTERMODE_UP,
            400,
            TIM_CLOCKDIVISION_DIV1,
            0,
            TIM_AUTORELOAD_PRELOAD_DISABLE
        }
    };

    void *handleTIM3 = &handle;

    static int frequency = 0;          // Частота звучащего в данное время звука
    static bool running = false;       // Если true, то звук звучит
}


void Beeper::Init()
{
    pinBEEP.Init();
}


void Beeper::Update()
{
    bool need_sound = false;

    for (int i = 0; i < NUM_MEASURES_TO_CONTROL; i++)
    {
//        Measure measure;

//        if (Storage::GetMeasure((Measure::E)i, measure) && !measure.InRange())
//        {
//            need_sound = true;
//            break;
//        }
    }

    need_sound ? Start(4000) : Stop(); //-V547
}


void Beeper::Beep(int _frequency, uint timeMS)
{
    Start(_frequency);

    Timer::Delay(timeMS);

    Stop();
}


void Beeper::Start(int _frequency)
{
    if (running && (_frequency == frequency))
    {
        return;
    }

    if (running)
    {
        Stop();
    }

    frequency = _frequency;

#ifndef WIN32
    __HAL_RCC_TIM3_CLK_ENABLE();
#endif

    handle.Init.Prescaler = (uint)(60000 / frequency - 1);

    HAL_NVIC_SetPriority(TIM3_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    HAL_TIM_Base_Init(&handle);
    HAL_TIM_Base_Start_IT(&handle);

    running = true;
}


void Beeper::Stop()
{
    HAL_TIM_Base_Stop_IT(&handle);
    HAL_TIM_Base_DeInit(&handle);
    HAL_NVIC_DisableIRQ(TIM3_IRQn);

#ifndef WIN32
    __HAL_RCC_TIM3_CLK_DISABLE();
#endif

    running = false;
}


bool Beeper::IsRunning()
{
    return running;
}


void Beeper::CallbackOnTimer()
{
    static bool level = true;

    level = !level;

    pinBEEP.Set(level);
}

