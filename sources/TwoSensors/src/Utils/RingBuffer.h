// 2023/10/16 10:59:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer.h"


template<int capacity>
class RingBuffer
{
public:
    RingBuffer() : index_in(0), index_out(0), num_elements(0) { }

    void Clear()
    {
        index_in = 0;
        index_out = 0;
        num_elements = 0;
    }

    void Append(uint8 byte)
    {
        if (GetElementCount() == capacity)
        {
            return;
        }

        buffer[index_in] = byte;
        index_in++;

        if (index_in == capacity)
        {
            index_in = 0;
        }

        num_elements++;
    }

    void GetData(Buffer<2048> &out)
    {
        if (num_elements == 0)
        {
            return;
        }

        int copied_bytes = num_elements;

        int index_new = index_out + copied_bytes;

        if (index_new >= capacity)
        {
            index_new -= capacity;
        }

        if (index_new > index_out)
        {
            out.Append(buffer + index_out, copied_bytes);
        }
        else
        {
            out.Append(buffer + index_out, capacity - index_out);
            out.Append(buffer, index_new);
        }

        index_out = index_new;

        num_elements -= copied_bytes;
    }

    uint8 Pop()
    {
        if (GetElementCount())
        {
            uint8 result = buffer[index_out++];
            if (index_out == capacity)
            {
                index_out = 0;
            }
            num_elements--;
            return result;
        }

        return 0;
    }

    int GetElementCount() const
    {
        return num_elements;
    }

    uint8 GetElement(int index)
    {
        if (index >= GetElementCount())
        {
            return 0;
        }

        int ret_index = index_out + index;

        if (ret_index >= capacity)
        {
            ret_index -= capacity;
        }

        return buffer[ret_index];
    }

    int GetCapacity() const
    {
        return capacity;
    }

    void RemoveFirstElements(int count)
    {
        if (count > GetElementCount())
        {
            count = GetElementCount();
        }

        index_out += count;

        if (index_out >= capacity)
        {
            index_out -= capacity;
        }

        num_elements -= count;
    }

private:
    uint8 buffer[capacity];
    int index_in;
    int index_out;
    int num_elements;
};
