// 2022/05/05 12:28:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Menu.h"
#include "Display/Display.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Display/Font/Font.h"
#include "Hardware/Beeper.h"


void Menu::ShortPress(const Key &key)
{
    if (!IsOpened())
    {
        if (key.Is1())
        {
            ++gset.display.typeDisplaydInfo;

            if (gset.display.typeDisplaydInfo.value == TypeDisplayedInformation::Count)
            {
                gset.display.typeDisplaydInfo.value = TypeDisplayedInformation::Temperature;
            }
        }
        else if (key.Is2())
        {
            PageMain::self->Open();
        }
    }
    else
    {
        Item::Opened()->ShortPressure(key);
    }

    Display::need_redraw = true;
}


void Menu::LongPress(const Key &key)
{
    if (!IsOpened())
    {
        if (key.Is1())
        {
            Display::Mode::EnableCompass(!Display::Mode::IsEnabledCompass());
        }
    }
    else
    {
        Item::Opened()->LongPressure(key);
    }

    Display::need_redraw = true;
}


void Menu::DoubleClick(const Key &)
{
    if (IsOpened())
    {
        Item::Opened()->DoubleClick();
    }

    Display::need_redraw = true;
}


bool Menu::IsOpened()
{
    return (Item::Opened() != &Page::Empty);
}


void Menu::Draw()
{
    Beeper::Stop();

    Display::BeginScene(Color::BLACK);

    Font::Set(TypeFont::_12_10);

    if (Item::Opened()->IsPage())
    {
        Item::Opened()->Draw(0, 0, true);
    }
    else
    {
        const Item *opened = Item::Opened();

        const Page *keeper = opened->Keeper();

        keeper->Draw(0, 0, false);

        Item::Opened()->DrawOpened(0, Menu::Title::HEIGHT + Item::Opened()->NumberOnPage() * Item::HEIGHT, true);
    }

    Display::EndScene();
}
