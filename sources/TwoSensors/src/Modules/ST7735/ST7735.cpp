// 2022/03/12 09:24:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/ST7735/ST7735.h"
#include "Hardware/Timer.h"
#include "Display/Font/Font.h"
#include "Utils/Text/String.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f3xx_hal.h>
#include <cstring>

// !!!!!!!!!!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// \warning В настройках файла должна быть отключена оптимизация -O0, иначе не работает
// !!!!!!!!!!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
    B11 - RST    Reset
    B14 - RS     D/C

    B12 - CS
    B13 - SCK    SCL
    B15 - SDA    MOSI
*/


const uint16 Color::colors[Color::Count] =
{
    0xffff,                     // WHITE
    0x0000,                     // BLACK
    MAKE_COLOR(31, 0, 0),       // RED
    MAKE_COLOR(0, 63, 0),       // GREEN
    MAKE_COLOR(0, 38, 5),       // _1
    MAKE_COLOR(0, 31, 0),       // GREEN_50
    MAKE_COLOR(0, 0, 31),       // BLUE
    MAKE_COLOR(15, 31, 15),
    MAKE_COLOR(7, 15, 7),
    MAKE_COLOR(3, 6, 3)
};


#define SPI2_DR_8bit         *(__IO uint8_t*)&(SPI2->DR)


namespace ST7735
{
#define SET_DC   pinDC_ST.ToHi()
#define RESET_DC pinDC_ST.ToLow()
#define SET_CS   pinCS_ST.ToHi()
#define RESET_CS pinCS_ST.ToLow()

    static SPI_HandleTypeDef handle;

    static void SendCommand(uint8);
    static void SendData8(uint8);
    static void SetWindow(int startX, int startY, int stopX, int stopY);
}


void ST7735::Init()
{
    __HAL_RCC_SPI2_CLK_ENABLE();

    pinSCL_SPI2.Init();
    pinMOSI_SPI2.Init();

    handle.Instance = SPI2;
    handle.Init.Mode = SPI_MODE_MASTER;
    handle.Init.Direction = SPI_DIRECTION_2LINES;
    handle.Init.DataSize = SPI_DATASIZE_8BIT;
    handle.Init.CLKPolarity = SPI_POLARITY_LOW;
    handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    handle.Init.NSS = SPI_NSS_SOFT;
    handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    handle.Init.TIMode = SPI_TIMODE_DISABLE;
    handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    handle.Init.CRCPolynomial = 7;

    HAL_SPI_Init(&handle);

    pinRESET_ST.ToLow();
    pinDC_ST.ToLow();
    pinCS_ST.ToHi();

    pinRESET_ST.Init();
    pinDC_ST.Init();
    pinCS_ST.Init();

    SPI2->CR1 |= SPI_CR1_SPE;

    pinCS_ST.ToLow();
    pinRESET_ST.ToHi(); //-V525
    HAL_Delay(20);
    pinRESET_ST.ToLow();
    HAL_Delay(20);
    pinRESET_ST.ToHi();
    HAL_Delay(20);

    SendCommand(0x01);      // SWRESET Software reset
    HAL_Delay(12);

    SendCommand(0x11);      // SLPOUT Sleep out
    HAL_Delay(12);

    SendCommand(0x3A);      // COLMOD Interface pixel format
    SendData8(0x05);        // 16 bit / pixel

    SendCommand(0x36);      // MADCTL Memory Data Access Control
    SendData8(BINARY_U8(01100000));

    Display::BeginScene(Color::BLACK);

    Display::EndScene();

    SendCommand(0x29);      // DISPON Display on
}


#ifndef GUI


#define WRITE_NIBBLE(nibble)    \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    asm("nop");                 \
    \
    SPI2->DR = Color::colors[value & 0x0f]; \
    value >>= 4


void ST7735::WriteBuffer(int x0, int y0, int width, int height)
{
    TimeMeterMS meter;

    SetWindow(x0, y0, width, height);

    SendCommand(0x2C);

    SPI2->CR2 &= ~SPI_CR2_DS_Msk;
    SPI2->CR2 |= SPI_DATASIZE_16BIT;

    SET_DC;
    RESET_CS;

    if ((x0 % 8) == 0 && ((width % 8) == 0))
    {
        for (int y = y0; y < y0 + height; y++)
        {
            uint* points = (uint *)Display::Buffer::GetLine(x0, y);

            uint value = *points;

            for (int i = 0; i < width; i += 8)
            {
                WRITE_NIBBLE(0);
                WRITE_NIBBLE(1);
                WRITE_NIBBLE(2);
                WRITE_NIBBLE(3);
                WRITE_NIBBLE(4);
                WRITE_NIBBLE(5);
                WRITE_NIBBLE(6);
                WRITE_NIBBLE(7);

                value = *(++points); //-V519
            }
        }
    }
    else
    {
        for (int y = y0; y < y0 + height; y++)
        {
            uint8* points = Display::Buffer::GetLine(x0, y);

            uint8 value = *points;

            for (int i = 0; i < width; i += 2)
            {
                WRITE_NIBBLE(0); //-V760
                WRITE_NIBBLE(1);

                value = *(++points); //-V519
            }
        }
    }

    SET_CS;

    SPI2->CR2 &= ~SPI_CR2_DS_Msk;
    SPI2->CR2 |= SPI_DATASIZE_8BIT;
}


void ST7735::WritePoint(int x, int y, uint16 color)
{
    SetWindow(x, y, 2, 2);

    SendCommand(0x2C);

    SPI2->CR2 &= ~SPI_CR2_DS_Msk;
    SPI2->CR2 |= SPI_DATASIZE_16BIT;

    SET_DC;
    RESET_CS;

    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    SPI2->DR = color;

    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    SPI2->DR = color;

    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    SPI2->DR = color;

    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    SPI2->DR = color;

    SET_CS;

    SPI2->CR2 &= ~SPI_CR2_DS_Msk;
    SPI2->CR2 |= SPI_DATASIZE_8BIT;
}


void ST7735::SetWindow(int x, int y, int width, int height)
{
    SendCommand(0x2A);      // CASET
    SendData8(0x00);
    SendData8((uint8)x);
    SendData8(0x00);
    SendData8((uint8)(x + width - 1));
    SendCommand(0x2B);      // RASET
    SendData8(0x00);
    SendData8((uint8)y);
    SendData8(0x00);
    SendData8((uint8)(y + height));
}

#endif


void ST7735::SendData8(uint8 data)
{
    __HAL_SPI_ENABLE(&handle);

    TimeMeterMS meter;

    SET_DC;
    RESET_CS;

    while ((SPI2->SR & SPI_SR_BSY))
    {
        if (meter.ElapsedTime() > 100)
        {
            break;
        }
    }

    while (!(SPI2->SR & SPI_SR_TXE))
    {
        if (meter.ElapsedTime() > 100)
        {
            break;
        }
    }

    SPI2_DR_8bit = data;

    while (!(SPI2->SR & SPI_SR_TXE))
    {
        if (meter.ElapsedTime() > 100)
        {
            break;
        }
    }

    while ((SPI2->SR & SPI_SR_BSY))
    {
        if (meter.ElapsedTime() > 100)
        {
            break;
        }
    }

    SET_CS;
}


void ST7735::SendCommand(uint8 data)
{
    __HAL_SPI_ENABLE(&handle);

    TimeMeterMS meter;

    RESET_DC;
    RESET_CS;

    while ((SPI2->SR & SPI_SR_BSY))
    {
        if (meter.ElapsedTime() > 100)
        {
            break;
        }
    }

    while (!(SPI2->SR & SPI_SR_TXE))
    {
        if (meter.ElapsedTime() > 100)
        {
            break;
        }
    }

    SPI2_DR_8bit = data;

    while (!(SPI2->SR & SPI_SR_TXE))
    {
        if (meter.ElapsedTime() > 100)
        {
            break;
        }
    }

    while ((SPI2->SR & SPI_SR_BSY))
    {
        if (meter.ElapsedTime() > 100)
        {
            break;
        }
    }

    SET_CS;
}
