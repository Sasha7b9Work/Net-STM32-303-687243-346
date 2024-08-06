// 2023/06/05 14:51:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include <stm32f3xx_hal.h>
#include <cstring>


static const uint ADDR_FLASH_SECTOR_63 = ((uint)0x0801F800);
static const uint ADDR_SECTOR_SETTINGS = ADDR_FLASH_SECTOR_63;


bool HAL_ROM::LoadSettings(Settings *settings)
{
    uint *pointer = (uint *)(ADDR_SECTOR_SETTINGS); //-V566

    if (*pointer == (uint)(-1))
    {
        return false;
    }

    std::memcpy(settings, (const void *)(ADDR_SECTOR_SETTINGS), sizeof(Settings)); //-V566

    return true;
}


uint HAL_ROM::SaveSettings(const Settings *settings)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef itd =
    {
        FLASH_TYPEERASE_PAGES,
        ADDR_SECTOR_SETTINGS,
        1
    };

    uint error = 0;

    if (HAL_FLASHEx_Erase(&itd, &error) != HAL_OK)
    {
        HAL_FLASH_Lock();

        return HAL_FLASH_GetError();
    }

    uint address = ADDR_SECTOR_SETTINGS;

    uint *data = (uint *)settings;
    
    uint count = (sizeof(Settings) / sizeof(uint) + 1);

    for (uint i = 0; i < count; i++)
    {
        while (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *data) != HAL_OK)
        {
            FLASH->CR &= (uint)(~(1 << 6));

            FLASH->SR &= (uint)(1 << 2);
        }

        address += 4;
        data++;
    }

    HAL_FLASH_Lock();

    return 0;
}

