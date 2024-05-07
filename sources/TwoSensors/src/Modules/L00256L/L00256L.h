// 2024/05/06 15:43:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace L00256L
{
    void Init();

    void Update();

    void CallbackOnInterrupt();

    bool IsEnabled();

    float GetAngleFull();

    float GetAngleRelative();

    float GetVelocity();
}
