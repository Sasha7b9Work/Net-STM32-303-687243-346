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
        static const int NUM_RECORDS = W25Q80DV::SIZE_SECTOR / sizeof(Record);
    };

    struct Memory
    {
        Sector sectors[W25Q80DV::NUM_SECTORS];

        void Init();

    private:

    friend struct Sector;

        uint number_oldest = UINT_MAX;
        int index_oldest = INT_MAX;         // Сквозной индекс самой старой записи Record

        int index_newest = INT_MIN;         // Сквозной индекс записи Record с наибольшим номером
        uint number_newest = 0;             // И номер данной записи

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
            bool IsEmpty() const;

            // Читает запись Record. Нумерация с начала сектора
            void ReadRecord(int number, Record *);
        };

        MemorySector data_sector;
    };

    static Memory memory;
}


void Storage::Memory::Init()
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
                for (int num_record = 0; num_record < Sector::NUM_RECORDS; num_record++)
                {
                    Record record;

                    data_sector.ReadRecord(num_record, &record);

                    if (record.IsValid())
                    {
                        int index = Sector::NUM_RECORDS * num_sector + num_record;          // Сквозной индекс Record

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


void Storage::Memory::MemorySector::Prepare(int _number)
{
    if (number != _number)
    {
        number = _number;

        buffer.Read((uint)number * W25Q80DV::SIZE_SECTOR);
    }
}


bool Storage::Memory::MemorySector::IsEmpty() const
{
    memory.data_sector.Prepare(number);

    {
        // Сначала проверим значения. Если все байты равны 0xFF, то в сектор после стирания ничего не записывалось

        int num_not_FF = 0;                 // Количество байт, не равных 0xFF

        for (int i = 0; i < buffer.Size(); i++)
        {
            if (memory.data_sector.buffer.Data()[i] != 0xFF)
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


    }

    return true;
}


void Storage::Init()
{
    index_in = 0;
    index_out = 0;
    num_elements = 0;

    memory.Init();
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
