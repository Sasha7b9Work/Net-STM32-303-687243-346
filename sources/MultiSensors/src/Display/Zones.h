// 2023/10/09 11:47:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Text/String.h"


class Zone
{
public:
    int x;
    int y;
    int width;
    int height;
    Color::E background;

    Zone(int _x, int _y, int _w, int _h, Color::E back) : x(_x), y(_y), width(_w), height(_h), background(back) { }

    virtual ~Zone() {}

    virtual void Draw() = 0;

protected:

    void Clear();

    void Send();
};


class ZoneFPS : public Zone
{
public:

    virtual ~ZoneFPS() override { }

    String<> string;

    ZoneFPS() : Zone(136, 119, 24, 8, Color::BLACK) { }

    virtual void Draw() override;
};
