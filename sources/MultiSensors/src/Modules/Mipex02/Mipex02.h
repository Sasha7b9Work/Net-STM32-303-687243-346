// 2025/09/18 08:50:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Measures.h"


// ћодуль дл€ измерени€ концентрации метана


namespace Mipex02
{
    bool Init();

    bool IsExist();

    void Update();

    bool GetMeasure(Measure *);

    void CallbackOnReceive(pchar);
}
