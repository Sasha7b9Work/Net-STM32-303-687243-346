// 2022/05/05 12:28:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Hardware/Keyboard.h"


namespace Menu
{
    void ShortPress(const Key &);

    void LongPress(const Key &);

    void DoubleClick(const Key &);

    void Draw();

    bool IsOpened();

    namespace Title
    {
        const int HEIGHT = 27;
    }
}
