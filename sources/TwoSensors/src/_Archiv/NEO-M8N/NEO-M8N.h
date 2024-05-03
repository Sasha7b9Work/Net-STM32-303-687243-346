// 2023/04/04 08:29:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Measures.h"


namespace NEO_M8N
{
    void Update();

    void CallbackOnReceive();

    bool IsReady();

    //                         широта             долгота             высота
    void GetMeasures(Measure *latitude, Measure *longitude, Measure *altitude);
}
