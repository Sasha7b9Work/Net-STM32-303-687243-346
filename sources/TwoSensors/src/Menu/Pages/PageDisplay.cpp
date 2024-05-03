// 2022/05/05 13:14:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


extern const DPage pageMain;


static void ClosePageDisplay(bool)
{
    PageDisplay::self->Close();
}

DEF_BUTTON(bClosePageDisplay,
    "Закрыть",
    *PageDisplay::self,
    ClosePageDisplay,
    nullptr,
    nullptr
)

DEF_PAGE_1(pageDisplay, //-V1027
    "ДИСПЛЕЙ",
    pageMain,
    nullptr,
    nullptr,
    bClosePageDisplay
)

const Page * const PageDisplay::self = (const Page *)&pageDisplay;
