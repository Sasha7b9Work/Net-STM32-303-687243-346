// 2023/04/12 09:30:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include <stm32f3xx_hal.h>


HAL_StatusTypeDef HAL_RTC_Init(RTC_HandleTypeDef * /*hrtc*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef * /*hrtc*/, RTC_TimeTypeDef * /*sTime*/, uint32_t /*Format*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef * /*hrtc*/, RTC_TimeTypeDef * /*sTime*/, uint32_t /*Format*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef */*hrtc*/, RTC_DateTypeDef * /*sDate*/, uint32_t /*Format*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef * /*hrtc*/, RTC_DateTypeDef * /*sDate*/, uint32_t /*Format*/)
{
    return HAL_OK;
}
