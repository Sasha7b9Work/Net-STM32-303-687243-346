// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct HC12
{
    static HC12 self;

    void Init();

    void Transmit(const void *buffer, int size);

    // ”правл€юща€ команда
    void Command(pchar);

private:

    struct RecvBuffer
    {
        static const int SIZE = 128;

        RecvBuffer() : pointer(0)
        {
        }

        void Push(char symbol)
        {
            if (pointer < SIZE)
            {
                data[pointer++] = symbol;
            }
        }

        char *Data()
        {
            return &data[0];
        }
        int NumSymbols()
        {
            return pointer;
        }
        void Clear()
        {
            pointer = 0;
        }

    private:
        char data[SIZE];
        int pointer;
    } recv_buffer;
};
