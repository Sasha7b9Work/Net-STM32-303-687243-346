// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"

/*
*    аждое изделие обладает уникальным ID, который вводитс€ после прошивки через меню
*/


int main(void)
{
    Device::Init();

    while (true)
    {
        Device::Update();
    }
}
