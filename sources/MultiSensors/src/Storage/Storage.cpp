// 2026/03/02 10:42:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Storage.h"


namespace Storage
{
    // Возвращает true, если хранилище полностью заполнено - ни одной записи больше не влезет
    static bool IsFull();

    // Стереть самую старую запись
    static void EraseOldestRecord();

    static void AppendRecord(const Record &);

    // Возвращает номер последней записи
    static uint NumberLastRecord();

    // Копирует в параметр последнюю запись. Если записей нет - возвращаемое значение false
    static bool LastRecord(Record &);
}


void Storage::Init()
{

}


void Storage::Update()
{

}


void Storage::AppendMeasure(const Measure &measure)
{
    if (IsFull())
    {
        EraseOldestRecord();
    }

    Record record;
    record.number = NumberLastRecord() + 1;
    record.time = HAL_RTC::GetTime();
    record.measure = measure;
    record.tail = 0;

    AppendRecord(record);
}


void Storage::EraseOldestRecord()
{
    #pragma message("Storage::EraseOldestRecord() not implemented")
}


void Storage::AppendRecord(const Record &)
{
    #pragma message("Storage::AppendRecord() not implemented")
}


bool Storage::IsFull()
{
    #pragma message("Storage::IsFull() not implemented")
    return true;
}


uint Storage::NumberLastRecord()
{
    Record record;

    if (LastRecord(record))
    {
        return record.number;
    }

    return 0;
}


bool Storage::LastRecord(Record &)
{
    #pragma message("Storage::LastRecord() not implemented")
    return false;
}
