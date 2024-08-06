// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace HC12
{
    void Init();

    void Transmit(const void *buffer, int size);

    // ”правл€юща€ команда
    void Command(pchar);

    void CallbackOnReceive();
}
