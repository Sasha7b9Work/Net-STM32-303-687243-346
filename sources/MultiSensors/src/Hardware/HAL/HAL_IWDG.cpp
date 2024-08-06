// 2023/12/07 16:56:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f3xx_hal.h>


namespace HAL_IWDG
{
    // Время срабатывания около одной секунды

    static IWDG_HandleTypeDef handle =
    {
        IWDG,
        {
            IWDG_PRESCALER_64,  // Prescaler
            1024,               // Reload
            IWDG_WINDOW_DISABLE // Windows
        }
    };
}


void HAL_IWDG::Init()
{
#ifndef WIN32

    __HAL_RCC_CLEAR_RESET_FLAGS();

    HAL_IWDG_Init(&handle);

#endif
}


void HAL_IWDG::Update()
{
#ifndef WIN32

    HAL_IWDG_Refresh(&handle);

#endif
}
