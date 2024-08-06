// 2023/09/08 22:13:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Storage.h"


struct Record
{
    static const int SIZE = sizeof(Measurements);

    Record(uint _address = (uint)-1) : address(_address) { }

    // Возможна запись сюда. Все байты равны 0xFF
    bool IsEmpty();

    // Стёртая запись, это когда на месте номера записан 0
    bool IsErased();

    // Есть данные и они корректны
    bool IsCorrect();

    void Erase();

    // Имеет ли смысл данныая запись
    bool IsExist() const { return address < (W25Q80DV::SIZE - Record::SIZE); }

    void Write(Measurements &, int number);

    const Measurements &GetMeasurements() const;

    uint GetAddress() const { return address; }

    int GetNumber() const { return GetMeasurements().number; }

    Record GetNextRecord() const;

    static int GetNextNumber();

private:

    struct ValueMeasurements
    {
        Measurements measurements;      // Здесь измерения, хранящиеся по адресу address
        bool is_valid = false;          // true, если измерения загужены
        const Measurements &GetMeasurements(uint address) const;
    };

    ValueMeasurements value_meas;

    uint address;
};


namespace MemoryStorage
{
    void Init();

    void Append(Measurements &);

    // Возвращает указатель на самую старую структуру данных (которая считана раньше всех). После использования нужно вызвать Erase()
    // с этим указателем, чтобы стереть структуру из хранилища
    bool GetOldest(Record *record);

    void Erase(Record *);
};
