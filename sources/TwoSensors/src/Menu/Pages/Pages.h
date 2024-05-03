// 2022/05/05 12:27:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Menu/MenuItemsDefs.h"


namespace PageMain
{
    extern const Page * const self;
};


namespace PageDisplay
{
    extern const Page * const self;
};


namespace PageMeasures
{
    extern const Page *const self;

    namespace Temperature
    {
        extern const Page *const self;
    }

    namespace Pressure
    {
        extern const Page *const self;
    }

    namespace Humidity
    {
        extern const Page *const self;
    }

    namespace DewPoint
    {
        extern const Page *const self;
    }
};


namespace PageHC12
{
    extern const Page *const self;
};


namespace PageSystem
{
    extern const Page *const self;
};


namespace PageDataBase
{
    extern const Page *const self;

    namespace Settings
    {
        extern const Page *const self;
    }

    namespace Browsing
    {
        extern const Page *const self;
    }
};
