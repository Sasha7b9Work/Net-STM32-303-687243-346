// 2026/03/03 12:19:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Storage/Record.h"


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
