// 2023/08/07 09:56:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


template<int size>
struct MemBuffer
{
    uint8 *Read(uint address);
    uint8 *Data() { return data + 4; }
private:
    // Резервируем на 4 байта больше чем нужно - сюда будет писать служебная информация для
    // записи во флеш-память
    uint8 data[size + 4];
};


namespace W25Q80DV
{
    static const uint BEGIN = 0;                // Адрес первого байта
    static const uint END = 1024 * 1024;
    static const uint SIZE = END - BEGIN;       // Количество байта
    static const int SIZE_SECTOR = 4 * 1024;
    static const int NUM_SECTORS = (END - BEGIN) / SIZE_SECTOR;

    template<int count>
    void WriteBuffer(uint address, const void *buffer);

    void ReadBuffer(uint address, int size, void *buffer);

    void ReadID(uint8 id[2]);

    // Стереть страницу, которой принадлежит address
    void EraseSectorForAddress(uint address);

    void ErasePage(int num_page);

    void WriteUInt(uint address, uint value);

    uint ReadUInt(uint address);

    void Write(uint address, uint8 byte);

    void WriteData(uint, const void *, int);

    uint8 ReadUInt8(uint address);

    void Clear();

    namespace Test
    {
        bool Run();
    }
}
