// 2026/03/02 10:41:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"


namespace Storage
{
    void Init();

    // Все отправления сообщений производятся отсюда
    void Update();

    // После каждого нового измерения добавляем его в хранилище вызовом этой функии
    void AppendMeasure(const Measure &);
}
