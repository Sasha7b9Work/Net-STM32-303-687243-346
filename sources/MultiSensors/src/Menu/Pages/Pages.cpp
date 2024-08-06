// 2022/05/05 12:26:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Storage/Measures.h"


extern const DPage pageMain;


static void CloseMainPage(bool)
{
    PageMain::self->Close();
}


DEF_BUTTON(bCloseMainPage,
    "Закрыть",
    pageMain,
    CloseMainPage,
    nullptr,
    nullptr
)


static void OnOpenClose_MainPage(bool open)
{
    if (open)
    {

    }
    else
    {
//        Settings::Save(&gset);
    }
}


DEF_PAGE_6(pageMain, //-V1027
    "МЕНЮ",
    Page::Empty,
    OnOpenClose_MainPage,
    nullptr,
    *PageMeasures::self,
    *PageDataBase::self,
    *PageDisplay::self,
    *PageHC12::self,
    *PageSystem::self,
    bCloseMainPage
)

const Page * const PageMain::self = (const Page *)&pageMain;
