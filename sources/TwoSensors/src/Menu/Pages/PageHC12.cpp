// 2022/05/05 14:53:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"


extern const DPage pageMain;


static void ClosePageHC12(bool)
{
    PageHC12::self->Close();
}

DEF_BUTTON( bClosePageHC12,
    "Закрыть",
    *PageHC12::self,
    ClosePageHC12,
    nullptr,
    nullptr
);

DEF_PAGE_1( pageHC12, //-V1027
    "HC12",
    pageMain,
    nullptr,
    nullptr,
    bClosePageHC12
)

const Page *const PageHC12::self = (const Page *)&pageHC12;
