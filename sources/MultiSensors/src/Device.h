// 2022/04/27 11:48:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Measures.h"


struct Device
{
    static Device self;

    void Init();

    void Update();

private:

    void ProcessMeasure(const Measure &, uint time);
};
