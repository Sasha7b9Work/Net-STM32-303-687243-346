// Sasha7b9@tut.by (c)
#pragma once
#include "Display/Display.h"
#include "Utils/Buffer.h"


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


class InterCom
{
public:

    static InterCom self;

    void SetDirection(Direction::E);

    void Send(const Measure &, uint timeMS);

private:

    Buffer<16> CreateMessage(const Measure &);

    Direction::E direction = Direction::_None;
};
