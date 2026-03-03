// 2026/03/03 12:10:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Memory.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Storage/Sector.h"


void Memory::Init()
{
    number_oldest_record = UINT_MAX;
    index_oldest_record = INT_MAX;

    index_newest_record = INT_MIN;
    number_newest_record = 0;

    for (int num_sector = 0; num_sector < W25Q80DV::NUM_SECTORS; num_sector++)
    {
        data_sector.Prepare(num_sector);

        if (!data_sector.IsEmpty())
        {
            for (int num_record = 0; num_record < Sector::CAPACITY_IN_RECORDS; num_record++)
            {
                Record record;

                data_sector.ReadRecord(num_record, &record);

                if (record.IsValid())
                {
                    int index = Sector::CAPACITY_IN_RECORDS * num_sector + num_record;          // Сквозной индекс Record

                    if (record.number < number_oldest_record)
                    {
                        number_oldest_record = record.number;
                        index_oldest_record = index;
                    }

                    if (record.number > number_newest_record)
                    {
                        number_newest_record = record.number;
                        index_newest_record = index;
                    }
                }
            }
        }
    }

}


bool Memory::IsFull()
{
    if (index_oldest_record == INT_MAX)        // Значит, нет ни одной записи
    {
        return false;
    }

    int number_sector_oldest = NumberSectorForIndexRecord(index_oldest_record);        // В этом секторе находится самая старая запись

    int number_record = NumberRecordForIndexRecord(number_sector_oldest, index_oldest_record);

    if (number_record == Sector::CAPACITY_IN_RECORDS - 1)       // Если данная запись последняя в секторе, нужно стереть
    {

    }

    return false;
}


int Memory::NumberSectorForIndexRecord(int index_record)
{
    return index_record / Sector::CAPACITY_IN_RECORDS;
}


int Memory::NumberRecordForIndexRecord(int num_sector, int num_record)
{
    return num_record % (num_sector * Sector::CAPACITY_IN_RECORDS);
}
