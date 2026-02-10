// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Modules/BME280/BME280.h"
#include "Modules/BH1750/BH1750.h"
#include "Modules/ST7735/ST7735.h"
#include "Modules/HI50/HI50.h"
#include "Modules/MQ135/SensorMQ135.h"
#include "Modules/MQ9/SensorMQ9.h"
#include "Modules/Mipex02/Mipex02.h"
#include "Hardware/Timer.h"
#include "Hardware/InterCom.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Beeper.h"
#include "Hardware/EnergySwitch.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"
#include "Modules/L00256L/L00256L.h"
#include "Hardware/HCDC/HCDC.h"
#include "Modules/W25Q80DV/W25Q80DV.h"


namespace Device
{
    static void ProcessMeasure(const Measure &, uint time);
}


void Device::Init()
{
    HAL::Init();

    GPIO_InitTypeDef is =
    {
        GPIO_PIN_2,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_HIGH,
        0
    };

    Timer::Delay(2000);

    HCDC::Init();

    HAL_GPIO_Init(GPIOA, &is);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

    ST7735::Init();

    EnergySwitch::Init();

    BME280::Init();

    BH1750::Init();

    if (!BME280::IsInitialized() && !BH1750::IsInitialized())
    {
        __HAL_RCC_I2C1_CLK_DISABLE();

        if (!Mipex02::Init())
        {
            if (!HI50::Init())
            {
                pinB6.Init();       // ѕо этим пинам
                pinB7.Init();       // будем определ€ть наличие не-I2C и не-USART модулей

                if (SensorMQ135::IsConnected())
                {
                    SensorMQ135::Init();
                }
                if (SensorMQ9::IsConnected())
                {
                    SensorMQ9::Init();
                }
                else if (L00256L::IsConnected())
                {
                    L00256L::Init();
                }
            }
        }
    }

    if (!HI50::IsExist() && !Mipex02::IsExist())        // ≈сли обнаружен дальномер, то не включаем HC12 на передачу - HI50 сам будет его включать,
    {                                                   // когда понадобитс€
        HAL_USART1::SetModeHC12();
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
    Measure concentrationCH4;
    Measure monoxide;
    Measure dioxide;

    uint time = TIME_MS;

    if (SensorMQ135::GetMeasure(&dioxide))
    {
        ProcessMeasure(dioxide, time);
    }

    if (SensorMQ9::GetMeasure(&monoxide))
    {
        ProcessMeasure(monoxide, time);
    }

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

    if (Mipex02::GetMeasure(&concentrationCH4))
    {
        InterCom::SetDirection(Direction::Display);
        ProcessMeasure(concentrationCH4, time);
        InterCom::SetDirection((Direction::E)(Direction::HC12 | Direction::Display));
    }

    if (!Menu::IsOpened())
    {
        Beeper::Update();
    }

    Keyboard::Update();

    Display::Update(TIME_MS);

    HAL_ADC::GetVoltageBattery();

    EnergySwitch::Update();

    HAL_USART1::Update();

    Mipex02::Update();

    SCPI::Update();

    L00256L::Update();

    uint8 buffer[64];

    uint len = HCDC::Update(buffer, sizeof(buffer));

    if (len > 0)
    {
        HCDC::RawTransmit(buffer, len);
    }
}


void Device::ProcessMeasure(const Measure &measure, uint time)
{
    if (measure.correct)
    {
        InterCom::Send(measure, time);
    }
}
