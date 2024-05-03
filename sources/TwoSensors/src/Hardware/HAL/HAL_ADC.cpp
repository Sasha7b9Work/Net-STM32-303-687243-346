// 2023/06/15 08:56:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f3xx_hal.h>
#include <cstring>


namespace HAL_ADC
{
    static ADC_HandleTypeDef handleADC;
    void *handle = (void *)&handleADC;
    static volatile bool flag_ready = false;

    static uint ReadChannel(uint channel);
}


void HAL_ADC::Init()
{
    pinADC.Init();
    pinHumidity.Init();

    handleADC.Instance = ADC1;
    handleADC.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
    handleADC.Init.Resolution = ADC_RESOLUTION_12B;
    handleADC.Init.ScanConvMode = ADC_SCAN_DISABLE;
    handleADC.Init.ContinuousConvMode = DISABLE;
    handleADC.Init.DiscontinuousConvMode = DISABLE;
    handleADC.Init.NbrOfDiscConversion = 0;
    handleADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    handleADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handleADC.Init.NbrOfConversion = 1;

    HAL_ADC_Init(&handleADC);

    HAL_NVIC_SetPriority(ADC1_IRQn, 1, 1);
}


uint HAL_ADC::ReadChannel(uint channel)
{
#ifdef GUI

    return 100;

#else
    ADC_ChannelConfTypeDef config = { 0 };

    config.Channel = channel;
    config.Rank = ADC_REGULAR_RANK_1;
    config.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;

    uint value = 0;

    if (HAL_ADC_ConfigChannel(&handleADC, &config) == HAL_OK)
    {
        HAL_NVIC_EnableIRQ(ADC1_IRQn);

        flag_ready = false;

        if (HAL_ADC_Start_IT(&handleADC) == HAL_OK)
        {
            while (!flag_ready)
            {
            }

            value = HAL_ADC_GetValue(&handleADC);
        }
    }

    return value;
#endif
}


float HAL_ADC::GetVoltage()
{
    static TimeMeterMS meter;

    static float voltage = 0.0f;

    if (meter.IsFinished())
    {
        float value = (float)ReadChannel(ADC_CHANNEL_4) * 3.3f * 1.25f / (float)(1 << 12);

        if (value > 3.0f)
        {
            voltage = value;
        }

        meter.FinishAfter(1000);
    }

    return voltage;
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


float HAL_ADC::GetHumidity()
{
    static Averager<8> averager;

    float voltage = (float)ReadChannel(ADC_CHANNEL_1) * 3.3f / (float)(1 << 12) + 0.075f;

    voltage = voltage * 3.0f / 2.0f;

    averager.Push(voltage);

    return averager.Get();
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *)
{
    HAL_ADC::flag_ready = true;
}
