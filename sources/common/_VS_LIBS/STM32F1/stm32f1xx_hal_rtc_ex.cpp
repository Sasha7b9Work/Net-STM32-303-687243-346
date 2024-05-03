// 2023/04/12 09:30:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include <stm32f1xx_hal.h>


void HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef * /*hrtc*/, uint32_t /*BackupRegister*/, uint32_t /*Data*/)
{

}


uint32_t HAL_RTCEx_BKUPRead(RTC_HandleTypeDef * /*hrtc*/, uint32_t /*BackupRegister*/)
{
    return 0;
}