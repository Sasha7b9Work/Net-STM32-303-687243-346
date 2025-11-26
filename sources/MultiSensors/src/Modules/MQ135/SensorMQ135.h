// 2025/11/26 10:57:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Measures.h"


// Измеряет концентрацию углекислого газа


namespace SensorMQ135
{
    void Init();

    bool IsInited();

    bool GetMeasure(Measure *);

    bool IsConnected();
}
