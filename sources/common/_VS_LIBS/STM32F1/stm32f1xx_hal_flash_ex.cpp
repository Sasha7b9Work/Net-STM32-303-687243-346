// 2023/06/20 10:40:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include <stm32f1xx_hal.h>


HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *, uint32_t *)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_FLASH_Program(uint32_t, uint32_t, uint64_t)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_FLASH_Unlock(void)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_FLASH_Lock(void)
{
    return HAL_OK;
}
