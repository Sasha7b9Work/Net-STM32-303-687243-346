// 2023/09/08 22:14:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/MemoryStorage.h"
#include "Hardware/Timer.h"
#include "Modules/W25Q80DV/W25Q80DV.h"


namespace MemoryStorage
{
    struct Page
    {
        static const int SIZE = W25Q80DV::SIZE_PAGE;

        Page(uint) { }

    private:

//        uint address;
    };

    // Сюда считываем много памяти, чтобы потом работать с ОЗУ
    namespace Cache
    {
        static const int SIZE = 512;        // Больше нет смысла делать (при 512 память читается за 3256 мс, при 8192 - за 3149 мс)
        static uint8 buffer[SIZE] __attribute__((section("CCM_DATA")));

        static uint begin = (uint)-1;               // Начало памяти в кеше

        static bool IsDirty()
        {
            return begin >= W25Q80DV::SIZE;
        }

        static uint End()
        {
            uint end = begin + SIZE;

            if (end > W25Q80DV::SIZE)
            {
                end = W25Q80DV::SIZE;
            }

            return end;
        }

        static void MarkAsDirty()
        {
            begin = (uint)-1;
        }

        // Возвращает true, если содержит память из адреса address, размером size
        static bool Consist(uint address, int size)
        {
            if (IsDirty() || begin > address)
            {
                return false;
            }

            return address + (uint)size < End();
        }

        // Заполнить содержимым памяти, начиная с адреса address
        static void FillFromMemory(uint address)
        {
            begin = address;

            uint size = W25Q80DV::SIZE - begin;

            if (size > SIZE)
            {
                size = SIZE;
            }

            static int counter = 0;
            counter++;

            MemBuffer<SIZE> mem_buf;

            uint8 *src = mem_buf.Read(begin);

            std::memcpy(buffer, src, SIZE);
        }

        static void Read(uint address, void *out, int size)
        {
            if (!Consist(address, size))
            {
                FillFromMemory(address);
            }

            uint shift = address - begin;

            std::memcpy(out, &buffer[shift], (uint)size);
        }

        static void Write(uint address, const Measurements &meas)
        {
            W25Q80DV::WriteBuffer<sizeof(meas)>(address, &meas);

            if (Consist(address, sizeof(meas)))
            {
                MarkAsDirty();
            }
        }
    }

    // Подготовоить место в памяти для записи новой записи
    static void PrepareForRecord(const Record &);

    static Record first_record((uint)-1);
    static Record last_record((uint)-1);
}


int Record::GetNextNumber()
{
    if (!MemoryStorage::last_record.IsExist())
    {
        return 1;
    }

    return MemoryStorage::last_record.GetMeasurements().number + 1;
}


void MemoryStorage::Init()
{
    for (uint address = 0; address < W25Q80DV::SIZE; address += Record::SIZE)
    {
        Record record(address);

        if (record.IsExist() &&
            !record.IsEmpty() &&                // Если сюда уже что-то было записано
            !record.IsErased())                 // И не стёрто
        {
            if (record.IsCorrect())
            {                                   // Корректируем первую и последнюю записи
                if (!first_record.IsExist())
                {
                    first_record = record;
                }

                if (!last_record.IsExist())
                {
                    last_record = record;
                }

                if (record.GetNumber() > last_record.GetNumber())
                {
                    last_record = record;
                }

                if (record.GetNumber() < first_record.GetNumber())
                {
                    first_record = record;
                }
            }
            else
            {
                record.Erase();         // Стираем некорректную запись
            }
        }
    }
}


void MemoryStorage::Append(Measurements &meas)
{
    /*
    *       Принцип хранения.
    *   1. Каждая запись снабжается порядковым номером, который рассчитываетс таким образом:
    *       - Если хранилище пустое, то этот номер 1.
    *       - Если в хранилище уже есть записи, то этот номер будет "номер последней записи + 1"
    *   2. Ищем свободное место в памяти:
    *       - Если записей нет, то этот адрес будет 0
    *       - Если адрес последней записи больше адреса первой записи, то запись будет идти за последней записью и станет последней записью
    */

    int number = Record::GetNextNumber();

    Record record(0);

    if (last_record.IsExist())
    {
        if (last_record.GetAddress() > first_record.GetAddress())   // Если записи идут в прямом порядке - последняя идёт после первой
        {
            record = last_record.GetNextRecord();

            if (record.GetAddress() < last_record.GetAddress())     // Если следующая запись находится перед последней записью
            {
                MemoryStorage::PrepareForRecord(record);
            }
        }
        else
        {

        }
    }

    record.Write(meas, number);
}


bool Record::IsEmpty()
{
    static bool first = true;
    static Measurements empty_meas;

    if (first)
    {
        first = false;

        std::memset(&empty_meas, 0xFF, sizeof(empty_meas)); //-V780
    }

    const Measurements &meas = GetMeasurements();

    return std::memcmp((void *)&meas, &empty_meas, sizeof(meas)) == 0; //-V1014
}


bool Record::IsErased()
{
    return GetMeasurements().number == 0;
}


bool Record::IsCorrect()
{
    const Measurements &meas = GetMeasurements();

    return (meas.control_field == 0) && (meas.CalculateCRC() == meas.crc);
}


void Record::Erase()
{
    W25Q80DV::WriteUInt(address, 0);

    if (MemoryStorage::Cache::Consist(address, sizeof(uint)))
    {
        MemoryStorage::Cache::MarkAsDirty();
    }
}


const Measurements &Record::GetMeasurements() const
{
    return value_meas.GetMeasurements(address);
}


const Measurements &Record::ValueMeasurements::GetMeasurements(uint addr) const
{
    Record::ValueMeasurements *pointer = (Record::ValueMeasurements *)this;

    if (!is_valid)
    {
        pointer->is_valid = true;

        MemoryStorage::Cache::Read(addr, &pointer->measurements, sizeof(measurements));
    }

    return measurements;
}


void Record::Write(Measurements &meas, int number)
{
    meas.number = number;
    meas.crc = meas.CalculateCRC();

    MemoryStorage::Cache::Write(address, meas);

    MemoryStorage::last_record = *this;
}


Record Record::GetNextRecord() const
{
    uint addr_next = address + SIZE;

    uint addr_end = addr_next + SIZE;

    if (addr_end >= W25Q80DV::SIZE)
    {
        addr_next = 0;
    }

    return Record(addr_next);
}


void MemoryStorage::PrepareForRecord(const Record &record)
{
    // Если нулевой байт record попадает на нулевой байт любой страницы, то нужно стирать эту страницу

    int num_page = (int)(record.GetAddress() / W25Q80DV::SIZE_PAGE);
    int next_page = (int)((record.GetAddress() + Record::SIZE) / W25Q80DV::SIZE_PAGE);

    if (record.GetAddress() % W25Q80DV::SIZE_PAGE)
    {
        W25Q80DV::ErasePage(num_page);

        Cache::MarkAsDirty();
    }

    // Если нулевой байт record попдает на одну страницу, а последний - на следующую, то нужно стирать следующую страницу
    if (num_page != next_page)
    {
        W25Q80DV::ErasePage(next_page);

        Cache::MarkAsDirty();
    }
}
