// 2025/03/13 08:01:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Measures.h"


// Измеряет концентрацию углекислого газа


namespace SensMQ9
{
    void Init();

    bool IsInited();

    bool GetMeasure(Measure *);

    // Подключён данный датчик
    bool IsConnected();
}
