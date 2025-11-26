// 2023/06/15 08:56:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f3xx_hal.h>
#include <cstring>


namespace HAL_ADC
{
    static ADC_HandleTypeDef handleADC1;
    void *handle1 = (void *)&handleADC1;
    static volatile bool flag_ready1 = false;
    static uint ReadChannelADC1(uint channel);

    static ADC_HandleTypeDef handleADC3;
    void *handle3 = (void *)&handleADC3;
    static volatile bool flag_ready3 = false;
    static uint ReadChannelADC3(uint channel);
}


void HAL_ADC::Init()
{
    pinBattery.Init();
    pinHumidity.Init();
    pinMQ9.Init();

    {
        handleADC1.Instance = ADC1;
        handleADC1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
        handleADC1.Init.Resolution = ADC_RESOLUTION_12B;
        handleADC1.Init.ScanConvMode = ADC_SCAN_DISABLE;
        handleADC1.Init.ContinuousConvMode = DISABLE;
        handleADC1.Init.DiscontinuousConvMode = DISABLE;
        handleADC1.Init.NbrOfDiscConversion = 0;
        handleADC1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        handleADC1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        handleADC1.Init.NbrOfConversion = 1;

        HAL_ADC_Init(&handleADC1);

        HAL_NVIC_SetPriority(ADC1_IRQn, 1, 1);
    }

    {
        handleADC3.Instance = ADC3;
        handleADC3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
        handleADC3.Init.Resolution = ADC_RESOLUTION_12B;
        handleADC3.Init.ScanConvMode = ADC_SCAN_DISABLE;
        handleADC3.Init.ContinuousConvMode = DISABLE;
        handleADC3.Init.DiscontinuousConvMode = DISABLE;
        handleADC3.Init.NbrOfDiscConversion = 0;
        handleADC3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        handleADC3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        handleADC3.Init.NbrOfConversion = 1;

        HAL_ADC_Init(&handleADC3);

        HAL_NVIC_SetPriority(ADC3_IRQn, 1, 1);
    }
}


uint HAL_ADC::ReadChannelADC1(uint channel)
{
#ifdef GUI

    return 100;

#else
    ADC_ChannelConfTypeDef config =
    {
        channel,
        ADC_REGULAR_RANK_1,
        ADC_SAMPLETIME_601CYCLES_5
    };

    uint value = 0;

    if (HAL_ADC_ConfigChannel(&handleADC1, &config) == HAL_OK)
    {
        HAL_NVIC_EnableIRQ(ADC1_IRQn);

        flag_ready1 = false;

        if (HAL_ADC_Start_IT(&handleADC1) == HAL_OK)
        {
            while (!flag_ready1)
            {
            }

            value = HAL_ADC_GetValue(&handleADC1);
        }
    }

    return value;
#endif
}


uint HAL_ADC::ReadChannelADC3(uint channel)
{
#ifdef GUI

    return 100;

#else
    ADC_ChannelConfTypeDef config =
    {
        channel,
        ADC_REGULAR_RANK_1,
        ADC_SAMPLETIME_601CYCLES_5
    };

    uint value = 0;

    if (HAL_ADC_ConfigChannel(&handleADC3, &config) == HAL_OK)
    {
        HAL_NVIC_EnableIRQ(ADC3_IRQn);

        flag_ready3 = false;

        if (HAL_ADC_Start_IT(&handleADC3) == HAL_OK)
        {
            while (!flag_ready3)
            {
            }

            value = HAL_ADC_GetValue(&handleADC3);
        }
    }

    return value;
#endif
}


float HAL_ADC::GetVoltageBattery()
{
    static TimeMeterMS meter;

    static float voltage = 0.0f;

    if (meter.IsFinished())
    {
        float value = (float)ReadChannelADC1(ADC_CHANNEL_4) * 3.3f * 1.25f / (float)(1 << 12);

        if (value > 3.0f)
        {
            voltage = value;
        }

        meter.FinishAfter(1000);
    }

    return voltage;
}


uint HAL_ADC::GetCarbonRaw()
{
    return ReadChannelADC3(ADC_CHANNEL_1);
}


template<int size_buffer>
class Averager //-V730
{
public:
    Averager() : num_elements(0) { }

    void Push(float value)
    {
        if (num_elements == size_buffer)
        {
            std::memmove(buffer, buffer + 1, sizeof(float) * (size_buffer - 1));

            num_elements--;
        }
        buffer[num_elements++] = value;
    }

    float Pop(int index)
    {
        return buffer[index];
    }

    float Get()
    {
        float sum = 0;

        for (int i = 0; i < num_elements; i++)
        {
            sum += buffer[i];
        }

        return sum / (float)num_elements;
    }
    int NumElements() const { return num_elements; }
    void Reset() { num_elements = 0; }
private:
    float buffer[size_buffer];
    int num_elements;
};


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *handle)
{
    if (handle == &HAL_ADC::handleADC1)
    {
        HAL_ADC::flag_ready1 = true;
    }
    else if (handle == &HAL_ADC::handleADC3)
    {
        HAL_ADC::flag_ready3 = true;
    }
}
