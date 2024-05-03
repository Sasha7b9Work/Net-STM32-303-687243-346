// Sasha7b9@tut.by (c)
#pragma once
#include "Display/Display.h"


struct Direction
{
    enum E
    {
        _None   = 0,
        CDC     = (1 << 0),
        HC12    = (1 << 1),
        Display = (1 << 2)
    };
};


namespace InterCom
{
    void SetDirection(Direction::E);

    void Send(const Measure &, uint timeMS);
}
