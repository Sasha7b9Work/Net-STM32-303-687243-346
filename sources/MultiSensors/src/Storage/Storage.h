// 2026/03/02 10:41:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"


struct Record
{
    uint       crc;
    uint       number;      // Порядковый номер записи. Нужно для нахождения последней и первой
    PackedTime time;
    Measure    measure;
    uint       tail;        // Сюда должен быть записан 0. При чтении мы читаем это значение. Если считан ноль,
                            // то запись была произведена полностью - значение правильное

    uint CalculateCRC() const;
};


struct Storage
{
    static Storage self;

    void Init();

    // Все отправления сообщений производятся отсюда
    void Update();

    // После каждого нового измерения добавляем его в хранилище вызовом этой функии
    void AppendMeasure(const Measure &);

private:

    // Возвращает true, если хранилище полностью заполнено - ни одной записи больше не влезет
    bool IsFull() const;

    // Стереть самую старую запись
    void EraseOldestRecord();

    void AppendRecord(const Record &);

    // Возвращает номер последней записи
    uint NumberLastRecord() const;
};
