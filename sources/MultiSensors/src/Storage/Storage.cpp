// 2026/03/02 10:42:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Storage.h"


Storage Storage::self;


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


uint Storage::NumberLastRecord() const
{
    #pragma message("Storage::NumberLastRecord() not implemented")

    return 0;
}


void Storage::AppendRecord(const Record &)
{
    #pragma message("Storage::AppendRecord() not implemented")
}


bool Storage::IsFull() const
{
    #pragma message("Storage::IsFull() not implemented")
    return true;
}
