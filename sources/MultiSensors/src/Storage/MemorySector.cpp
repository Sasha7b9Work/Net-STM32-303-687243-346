// 2026/03/03 12:19:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/MemorySector.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Storage/Sector.h"


void MemorySector::Prepare(int _number)
{
    if (number != _number)
    {
        number = _number;

        buffer.Read((uint)number * W25Q80DV::SIZE_SECTOR);
    }
}


bool MemorySector::IsEmpty()
{
    {
        // Сначала проверим значения. Если все байты равны 0xFF, то в сектор после стирания ничего не записывалось

        int num_not_FF = 0;                 // Количество байт, не равных 0xFF

        for (int i = 0; i < buffer.Size(); i++)
        {
            if (buffer.Data()[i] != 0xFF)
            {
                num_not_FF++;
                break;
            }
        }

        if (num_not_FF == 0)
        {
            return true;
        }
    }

    {
        // Теперь будем считывать записи и смотреть, есть ли хоть одна валидная

        for (int num_record = 0; num_record < Sector::CAPACITY_IN_RECORDS; num_record++)
        {
            Record record;

            ReadRecord(num_record, &record);

            if (record.IsValid())
            {
                return false;
            }
        }
    }

    return true;
}


void MemorySector::ReadRecord(int number_record, Record *record)
{
    std::memcpy(record, buffer.Data() + number_record * record->Size(), (uint)record->Size());
}
