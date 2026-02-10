// 2026/02/03 10:24:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace HCDC
{
    void Init();

    void Transmit(const void *buffer, int size);

    uint Update(uint8 *buf, uint max_len);

    // Отправить в CDC
    void RawTransmit(uint8 *buf, uint len);
}
