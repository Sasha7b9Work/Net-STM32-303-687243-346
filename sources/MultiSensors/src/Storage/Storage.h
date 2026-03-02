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


namespace Storage
{
    void Init();

    // Все отправления сообщений производятся отсюда
    void Update();

    // После каждого нового измерения добавляем его в хранилище вызовом этой функии
    void AppendMeasure(const Measure &);
}
