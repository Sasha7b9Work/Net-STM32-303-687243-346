// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Modules/BME280/BME280.h"
#include "Modules/BH1750/BH1750.h"
#include "Modules/ST7735/ST7735.h"
#include "Modules/HI50/HI50.h"
#include "Hardware/Timer.h"
#include "Hardware/InterCom.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Beeper.h"
#include "Hardware/EnergySwitch.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"


namespace Device
{
    static void ProcessMeasure(const Measure &, uint time);
}


void Device::Init()
{
    HAL::Init();

    ST7735::Init();

    EnergySwitch::Init();

    if (!BME280::Init())                    // Пытаемся инициализировать датчик давления
    {
        if (!BH1750::Init())                // Если нет - то освещённость
        {
            __HAL_RCC_I2C1_CLK_DISABLE();
            
            if (!HI50::Init())              // Если нет - то датчик дальности
            {
                Keyboard::EnableEncoder();  // И в последнюю очередь запускаем энкодер
            }
        }
    }

    Keyboard::Init();

    Beeper::Init();

    InterCom::SetDirection((Direction::E)(Direction::HC12 | Direction::Display));
}


void Device::Update()
{
    Measure temp;
    Measure pressure;
    Measure humidity;
    Measure dew_point;
    Measure illuminate;
    Measure distance;

    uint time = TIME_MS;

    if (BME280::GetMeasures(&temp, &pressure, &humidity, &dew_point))
    {
        ProcessMeasure(temp, time);
        ProcessMeasure(pressure, time);
        ProcessMeasure(humidity, time);
        ProcessMeasure(dew_point, time);
    }

    if (BH1750::GetMeasure(&illuminate))
    {
        ProcessMeasure(illuminate, time);
    }

    if (HI50::GetMeasure(&distance))
    {
        ProcessMeasure(distance, time);
    }

    if (!Menu::IsOpened())
    {
        Beeper::Update();
    }

    Keyboard::Update();

    Display::Update(TIME_MS);

    HAL_ADC::GetVoltage();

    EnergySwitch::Update();

    HAL_USART_HI50::Update();

    HI50::Update();

    SCPI::Update();
}


void Device::ProcessMeasure(const Measure &measure, uint time)
{
    if (measure.correct)
    {
        InterCom::Send(measure, time);
    }
}
