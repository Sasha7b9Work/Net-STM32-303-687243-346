// 2026/03/02 10:42:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Storage.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Utils/Math.h"
#include <climits>


namespace Storage
{
    struct Record
    {
        uint       crc;
        uint       number;      // Порядковый номер записи. Нужно для нахождения последней и первой
        PackedTime time;
        Measure    measure;

        uint CalculateCRC() const;
        bool IsValid() const;
        int Size() const;
        uint8 *Begin() const
        {
            return (uint8 *)this;
        }
        // Начало (за исключением crc)
        uint8 *BeginData() const
        {
            return Begin() + sizeof(crc);
        }
        // Размер данных (за исключением crc)
        int SizeData() const
        {
            return sizeof(*this) - sizeof(crc);
        }
    };

    // Стереть самую старую запись
    static void EraseOldestSector();

    static void AppendRecord(const Record &);

    // Возвращает номер последней записи
    static uint NumberLastRecord();

    // Копирует в параметр последнюю запись. Если записей нет - возвращаемое значение false
    static bool LastRecord(Record &);

    static const int NUM_RECORDS_IN_SECTOR = W25Q80DV::SIZE_SECTOR / sizeof(Record);

    // Возвращает номер записи по её сквозному индексу
    static int NumberRecordForIndexRecord(int num_sector, int num_record);

    // Возвращает true, если хранилище полностью заполнено - ни одной записи больше не влезет
    bool IsFull();

    int index_oldest = INT_MAX;         // Сквозной индекс самой старой записи Record
    uint number_oldest = UINT_MAX;      // И номер данной записи

    int index_newest = INT_MIN;         // Сквозной индекс записи Record с наибольшим номером
    uint number_newest = 0;             // И номер данной записи

    // Возвращает номер сектора, в котором хранится запись с данным индексом
    static int NumberSectorForIndexRecord(int);

    // В этой структуре хранится последний считанный из памятии сектор
    struct MemorySector
    {
        int number = -1;                            // Номер сектора
        MemBuffer<W25Q80DV::SIZE_SECTOR> buffer;    // Данные сектора

        // Считывает данные из микросхемы
        void Prepare(int number);

        // Записывает данные в микросхему. start - от начала сектора, size - сколько байт записать
        void WriteData(uint start, int size);

        // Возвращает true, если нет ни одной валидной записи
        bool IsEmpty();

        // Читает запись Record. Нумерация с начала сектора
        void ReadRecord(int number, Record *);
    };

    MemorySector data_sector;
}


uint Storage::Record::CalculateCRC() const
{
    return Math::CalculateCRC(BeginData(), SizeData());
}


int Storage::NumberSectorForIndexRecord(int index_record)
{
    return index_record / NUM_RECORDS_IN_SECTOR;
}


bool Storage::Record::IsValid() const
{
    return  crc == CalculateCRC();
}


int Storage::Record::Size() const
{
    return sizeof(*this);
}


int Storage::NumberRecordForIndexRecord(int num_sector, int num_record)
{
    return num_record % (num_sector * NUM_RECORDS_IN_SECTOR);
}


bool Storage::IsFull()
{
    if (index_oldest == INT_MAX)        // Значит, нет ни одной записи
    {
        return false;
    }

    int number_sector_oldest = NumberSectorForIndexRecord(index_oldest);        // В этом секторе находится самая старая запись

    int number_record = NumberRecordForIndexRecord(number_sector_oldest, index_oldest);

    if (number_record == NUM_RECORDS_IN_SECTOR - 1)       // Если данная запись последняя в секторе, нужно стереть
    {

    }

    return false;
}


void Storage::MemorySector::Prepare(int _number)
{
    if (number != _number)
    {
        number = _number;

        buffer.Read((uint)number * W25Q80DV::SIZE_SECTOR);
    }
}


bool Storage::MemorySector::IsEmpty()
{
    {
        // Сначала проверим значения. Если все байты равны 0xFF, то в сектор после стирания ничего не записывалось

        int num_not_FF = 0;                 // Количество байт, не равных 0xFF

        for (int i = 0; i < buffer.Size(); i++)
        {
            if (data_sector.buffer.Data()[i] != 0xFF)
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

        for (int num_record = 0; num_record < NUM_RECORDS_IN_SECTOR; num_record++)
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


void Storage::MemorySector::ReadRecord(int number_record, Record *record)
{
    std::memcpy(record, buffer.Data() + number_record * record->Size(), (uint)record->Size());
}


void Storage::Init()
{
    number_oldest = UINT_MAX;
    index_oldest = INT_MAX;

    index_newest = INT_MIN;
    number_newest = 0;

    {
        for (int num_sector = 0; num_sector < W25Q80DV::NUM_SECTORS; num_sector++)
        {
            data_sector.Prepare(num_sector);

            if (!data_sector.IsEmpty())
            {
                for (int num_record = 0; num_record < NUM_RECORDS_IN_SECTOR; num_record++)
                {
                    Record record;

                    data_sector.ReadRecord(num_record, &record);

                    if (record.IsValid())
                    {
                        int index = NUM_RECORDS_IN_SECTOR * num_sector + num_record;          // Сквозной индекс Record

                        if (record.number < number_oldest)
                        {
                            number_oldest = record.number;
                            index_oldest = index;
                        }

                        if (record.number > number_newest)
                        {
                            number_newest = record.number;
                            index_newest = index;
                        }
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
    Record record;

    if (LastRecord(record))
    {
        return record.number;
    }

    return 0;
}


bool Storage::LastRecord(Record &)
{
    return false;
}
