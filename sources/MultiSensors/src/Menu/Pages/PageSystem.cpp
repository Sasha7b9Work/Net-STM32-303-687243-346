// 2022/08/04 14:32:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"


extern const DPage pageMain;


namespace PageSystem
{
    static int cur_field = 0;
    static int state = 0;
    static PackedTime time;
}


static void OnClose_Battery(bool)
{

}


static void OnDraw_Battery(int x, int y, Color::E, Color::E color_draw)
{
    String<>("%.2f В", (double)HAL_ADC::GetVoltage()).Draw(x + 90, y + 5, color_draw);
}


DEF_BUTTON(bBattery,
    "Батарея",
    *PageSystem::self,
    OnClose_Battery,
    OnDraw_Battery,
    nullptr
)


static void Before_OpenTime(bool open)
{
    if (open)
    {
        PageSystem::time = HAL_RTC::GetTime();
    }
}

DEF_TIMEITEM(tTime, *PageSystem::self, Before_OpenTime, PageSystem::cur_field, PageSystem::state, PageSystem::time)


static void OnDraw_SerialNumber(int x, int y, Color::E color_fill, Color::E color_draw)
{
    Rectangle(6, 12).Fill(x + 143, y + 5, color_fill);

    String<> serial_number("%08X", HAL::GetUID());

    serial_number.Draw(x + 85, y + 5, color_draw);
}


static int null_value = 0;


DEF_GOVERNOR(gSerialNumber,
    "С/Н",
    *PageSystem::self,
    nullptr,
    OnDraw_SerialNumber,
    0,
    0,
    null_value
)


static void ClosePageSystem(bool)
{
    PageSystem::self->Close();
}


DEF_BUTTON(bClosePageSystem,
    "Закрыть",
    *PageSystem::self,
    ClosePageSystem,
    nullptr,
    nullptr
);


DEF_PAGE_4(pageSystem, //-V1027
    "СИСТЕМА",
    pageMain,
    nullptr,
    nullptr,
    bBattery,
    tTime,
    gSerialNumber,
    bClosePageSystem
)


const Page *const PageSystem::self = (const Page *)&pageSystem;
