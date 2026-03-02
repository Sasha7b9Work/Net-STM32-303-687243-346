// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/HC12/HC12.h"
#include "stm32f3xx_hal.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <cstring>


HC12 HC12::self;


namespace _HC12
{
#define PORT_SET GPIOA
#define PIN_SET  GPIO_PIN_6
}


void HC12::Init()
{
    pinCS_HC12.Init();
    pinCS_HC12.ToHi();

    Command("AT+DEFAULT");
    Command("AT");
}


void HC12::Transmit(const void *buffer, int size)
{
    HAL_USART1::Send(buffer, size);
}


void HC12::Command(pchar command)
{
    pinCS_HC12.ToLow();

    TimeMeterMS().PauseOnMS(40);

    recv_buffer.Clear();

    Transmit(command, (int)std::strlen(command));
    Transmit("\r", 1);

    pinCS_HC12.ToHi();
}
