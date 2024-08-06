// 2023/01/30 15:35:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f3xx_hal.h>
#include <ctime>


namespace HAL_RTC
{
    static RTC_HandleTypeDef handleRTC =
    {
        RTC,
        {
            RTC_HOURFORMAT_24,
            127,
            255,
            RTC_OUTPUT_DISABLE,
            RTC_OUTPUT_POLARITY_HIGH,
            RTC_OUTPUT_TYPE_OPENDRAIN
        }
    };
}


void HAL_RTC::Init()
{
    __HAL_RCC_RTC_ENABLE();

    HAL_RTC_Init(&handleRTC);

#define VALUE_FOR_RTC 0x644

    if (HAL_RTCEx_BKUPRead((RTC_HandleTypeDef *)&handleRTC, RTC_BKP_DR0) != VALUE_FOR_RTC)
    {
        SetTime(PackedTime(11, 11, 11, 11, 11, 11));

        HAL_RTCEx_BKUPWrite((RTC_HandleTypeDef *)&handleRTC, RTC_BKP_DR0, VALUE_FOR_RTC);
    }
}


PackedTime HAL_RTC::GetTime()
{
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;

    HAL_RTC_GetTime(&handleRTC, &time, RTC_HOURFORMAT_24);

    HAL_RTC_GetDate(&handleRTC, &date, RTC_HOURFORMAT_24);

    return PackedTime(time.Hours, time.Minutes, time.Seconds, date.Date, date.Month, date.Year);
}


void HAL_RTC::SetTime(const PackedTime &pack_time)
{
    RTC_TimeTypeDef time =
    {
        (uint8)pack_time.hours,
        (uint8)pack_time.minutes,
        (uint8)pack_time.seconds
    };

    RTC_DateTypeDef date =
    {
        0,
        (uint8)pack_time.month,
        (uint8)pack_time.day,
        (uint8)pack_time.year
    };

    HAL_RTC_SetTime(&handleRTC, &time, RTC_HOURFORMAT_24);

    HAL_RTC_SetDate(&handleRTC, &date, RTC_HOURFORMAT_24);
}


String<> PackedTime::ToString() const
{
    return String<>("%02d:%02d:%02d", hours, minutes, seconds);
}
