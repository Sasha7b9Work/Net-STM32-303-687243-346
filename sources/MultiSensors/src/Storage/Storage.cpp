// 2026/03/02 10:42:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Storage.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Utils/Math.h"
#include "Storage/Memory.h"
#include "Storage/Record.h"
#include "Storage/MemorySector.h"
#include "Storage/Sector.h"


namespace Storage
{
    // Стереть самую старую запись
    static void EraseOldestSector();

    static void AppendRecord(const Record &);

    // Возвращает номер последней записи
    static uint NumberLastRecord();

    // Копирует в параметр последнюю запись. Если записей нет - возвращаемое значение false
    static bool LastRecord(Record *);

    static Memory memory;
}


void Storage::Init()
{
    memory.Init();
}


void Storage::Update()
{

}


void Storage::AppendMeasure(const Measure &measure)
{
    if (memory.IsFull())
    {
        EraseOldestSector();
    }

    Record record;
    record.number = NumberLastRecord() + 1;
    record.time = HAL_RTC::GetTime();
    record.measure = measure;

    AppendRecord(record);
}


void Storage::EraseOldestSector()
{
}


void Storage::AppendRecord(const Record &)
{
}


uint Storage::NumberLastRecord()
{
    return number_newest_record;
}
