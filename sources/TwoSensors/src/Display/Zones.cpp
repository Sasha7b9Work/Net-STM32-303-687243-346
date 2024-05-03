// 2023/10/09 11:47:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Zones.h"
#include "Display/Display.h"
#include "Modules/ST7735/ST7735.h"


void Zone::Clear()
{
    Rectangle(width, height).Fill(x, y, background);
}


void Zone::Send()
{
    ST7735::WriteBuffer(x, y, width, height);
}


void ZoneFPS::Draw()
{
    Clear();

    string.Draw(x, y, Color::GRAY_10);

    Send();
}
