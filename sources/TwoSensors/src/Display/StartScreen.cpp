// 2023/10/03 11:23:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/StartScreen.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Modules/ST7735/ST7735.h"
#include <cstdlib>


namespace StartScreen
{
    struct Row
    {
        uint pixels[5];         // Здесь хранятся 150 пикселей

        int GetBit(int /*num_bit*/)
        {
//            int num_pixel = num_bit / 32;

            return 0;
        }

        void SetBit(int)
        {

        }

        void Init()
        {
            for (int i = 0; i < 5; i++)
            {
                pixels[i] = 0;
            }
        }

        bool AppendPixel()
        {
            for (int i = 0; i < 5; i++)
            {
                if (pixels[i] == (uint)(-1))
                {
                }
            }

            return false;
        }
    };

    struct ScreenBuffer
    {
        Row rows[128];

        void Init()
        {
            for (int i = 0; i < 128; i++)
            {
                rows[i].Init();
            }
        }

        // Возвращает false, если добавлять некуда
        bool AppendPixel()
        {
            for (int i = 0; i < 128; i++)
            {
                if (!rows[i].AppendPixel())
                {
                    return false;
                }
            }

            return true;
        }
    };
}


void StartScreen::Run()
{
    Display::BeginScene(Color::WHITE);

    Display::EndScene();

//    for (int i = 0; i < Display::HEIGHT; i++)
//    {
//        HLine(Display::WIDTH).Draw(0, i, Color::BLACK);
//
//        ST7735::WriteBuffer(0, i, Display::WIDTH, 1);
//    }

    for (int i = 0; i < 10000; i++)
    {
        ST7735::WritePoint((int)(std::rand() % Display::WIDTH), (int)(std::rand() % Display::HEIGHT), 0);
    }
}
