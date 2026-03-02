// 2026/03/02 10:42:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Storage.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include <climits>


namespace Storage
{
    struct Record
    {
        uint       crc;
        uint       number;      // Порядковый номер записи. Нужно для нахождения последней и первой
        PackedTime time;
        Measure    measure;
        uint       tail;        // Сюда должен быть записан 0. При чтении мы читаем это значение. Если считан ноль,
                                // то запись была произведена полностью - значение правильное

        uint CalculateCRC() const;
        bool IsValid() const;
        int Size() const;
        uint8 *Begin();
    };

    // Возвращает true, если хранилище полностью заполнено - ни одной записи больше не влезет
    static bool IsFull();

    // Стереть самую старую запись
    static void EraseOldestRecord();

    static void AppendRecord(const Record &);

    // Возвращает номер последней записи
    static uint NumberLastRecord();

    // Копирует в параметр последнюю запись. Если записей нет - возвращаемое значение false
    static bool LastRecord(Record &);

    static int index_in = 0;        // Под этим индексом будет сохранён следующий элемент
    static int index_out = 0;       // Индекс следующего считываемого элемента
                                    // В индексах сплошная нумерация по всем секторам. 0 соответствует элементу, расположенному по нулевому адресу нулевого сектора
                                    // Последний возможный индекс - у последнего элемента последнего сектора

    static int num_elements = 0;    // Количество сохранённых элементов

    struct Sector
    {
        uint start_address;

        int NumRecords() const
        {
            return W25Q80DV::SIZE_SECTOR / sizeof(Record);
        }

        // Возвращает true, если нет ни одной валидной записи
        bool IsEmpty() const;

        void ReadRecord(int num_record, Record *);
    };

    static Sector sectors[W25Q80DV::NUM_SECTORS];
}


bool Storage::Sector::IsEmpty() const
{
    return true;
}


void Storage::Init()
{
    index_in = 0;
    index_out = 0;
    num_elements = 0;

    for (uint i = 0; i < W25Q80DV::NUM_SECTORS; i++)
    {
        sectors[i].start_address = i * W25Q80DV::SIZE_SECTOR;
    }

    uint number_first = UINT_MAX;
    int index_first = INT_MAX;           // Сквозной индекс записи Recrod с наименьшим номером

    int index_last = INT_MIN;           // Сквозной индекс записи Record с наибольшим номером
    uint number_last = 0;               // И номер данной записи

    for (int num_sector = 0; num_sector < W25Q80DV::NUM_SECTORS; num_sector++)
    {
        Sector &sector = sectors[num_sector];

        if (!sector.IsEmpty())
        {
            for (int num_record = 0; num_record < sector.NumRecords(); num_record++)
            {
                Record record;

                sector.ReadRecord(num_record, &record);

                if (record.IsValid())
                {
                    int index = sector.NumRecords() * num_sector + num_record;          // Сквозной индекс Record

                    if (index < index_first)
                    {
                        index_first = index;
                        number_first = record.number;
                    }
                }
            }
        }
    }
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
