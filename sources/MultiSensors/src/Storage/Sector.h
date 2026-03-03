// 2026/03/03 12:14:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Storage/Record.h"


struct Sector
{
    static const int CAPACITY_IN_RECORDS = W25Q80DV::NUM_SECTORS / sizeof(Record);
};
