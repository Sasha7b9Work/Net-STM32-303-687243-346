// 2026/03/03 12:10:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/MemorySector.h"
#include <climits>


struct Memory
{
    void Init();

    // Возвращает true, если хранилище полностью заполнено - ни одной записи больше не влезет
    bool IsFull();

private:

    MemorySector data_sector;

    int index_oldest_record = INT_MAX;         // Сквозной индекс самой старой записи Record
    uint number_oldest_record = UINT_MAX;      // И номер данной записи

    int index_newest_record = INT_MIN;         // Сквозной индекс записи Record с наибольшим номером
    uint number_newest_record = 0;             // И номер данной записи

    // Возвращает номер сектора, в котором хранится запись с данным индексом
    int NumberSectorForIndexRecord(int);

    // Возвращает номер записи по её сквозному индексу
    int NumberRecordForIndexRecord(int num_sector, int num_record);
};
