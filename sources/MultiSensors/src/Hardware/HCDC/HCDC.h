// 2026/02/03 10:24:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// USB
namespace HCDC
{
    void Init();

    // Копирует принятые данные в buf. Возвращает количество принятых байт
    int GetReceivedData(uint8 *buf, uint max_len);

    // Отправить в CDC
    void RawTransmit(const void *buf, int len);
}
