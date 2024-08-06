// 2024/01/18 12:00:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"


extern const DPage pageMain;


static void ClosePageDataBaseSettings(bool)
{
    PageDataBase::Settings::self->Close();
}


DEF_BUTTON(bClosePageDataBaseSettings,
    "Çàêğûòü",
    *PageDataBase::Settings::self,
    ClosePageDataBaseSettings,
    nullptr,
    nullptr
)


DEF_PAGE_1(pageDataBaseSettings, //-V1027
    "ÍÀÑÒĞÎÉÊÈ",
    *PageDataBase::self,
    nullptr,
    nullptr,
    bClosePageDataBaseSettings
)


const Page *const PageDataBase::Settings::self = (const Page *)&pageDataBaseSettings;
