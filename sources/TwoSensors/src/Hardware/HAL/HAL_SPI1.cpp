// 2023/08/07 10:09:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f3xx_hal.h>


namespace HAL_SPI1
{
    static SPI_HandleTypeDef handle =
    {
        SPI1,
        {
            SPI_MODE_MASTER,                // Init.Mode
            SPI_DIRECTION_2LINES,           // Init.Direction
            SPI_DATASIZE_8BIT,              // Init.DataSize
            SPI_POLARITY_HIGH,              // Init.CLKPolarity
            SPI_PHASE_2EDGE,                // Init.CLKPhase
            SPI_NSS_SOFT,                   // Init.NSS
            SPI_BAUDRATEPRESCALER_2,        // Init.BaudRatePrescaler
            SPI_FIRSTBIT_MSB,               // Init.FirstBit
            SPI_TIMODE_DISABLE,             // Init.TIMode
            SPI_CRCCALCULATION_DISABLE,     // Init.CRCCalculation
            7,                              // InitCRCPolynomial
            SPI_CRC_LENGTH_8BIT,
            SPI_NSS_PULSE_DISABLE
        }
    };

    namespace WP
    {
        static void Init()
        {
            pinWP.Init();

            pinWP.ToLow();

            Timer::Delay(50);

            pinWP.ToHi();

            Timer::Delay(50);

            pinWP.ToLow();
        }
    }

    namespace CS
    {
        static void Set()
        {
            pinNSS_SPI1.ToHi();
        }

        static void Reset()
        {
            pinNSS_SPI1.ToLow();
        }
    }
}


void HAL_SPI1::Init()
{
    WP::Init();

    pinSCK_SPI1.Init();
    pinMOSI_SPI1.Init();
    pinMISO_SPI1.Init();
    pinNSS_SPI1.Init();

    CS::Set();

    HAL_SPI_Init(&handle);
}


void HAL_SPI1::WriteRead(const void *out, uint8 *in, int size)
{
    CS::Reset();

    HAL_SPI_TransmitReceive(&handle, (uint8 *)out, in, (uint16)size, 10000);

    CS::Set();
}


void HAL_SPI1::Write(const void *data, int size)
{
    CS::Reset();

    HAL_SPI_Transmit(&handle, (uint8 *)data, (uint16)size, 100);

    CS::Set();
}


void HAL_SPI1::Write(uint8 byte)
{
    Write(&byte, 1);
}
