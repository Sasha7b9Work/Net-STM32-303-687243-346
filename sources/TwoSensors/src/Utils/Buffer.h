#pragma once
#include <cstring>


template<int size_buffer>
class Buffer
{
public:

    Buffer() : size(size_buffer) {} //-V730

    Buffer(int _size) : size(size_buffer) //-V730
    {
        Realloc(_size);
    }

    Buffer(int _size, uint8 value) : size(size_buffer)
    {
        Realloc(_size);
        Fill(value);
    }

    void Fill(uint8 value)
    {
        for (int i = 0; i < size; i++)
        {
            buffer[i] = value;
        }
    }

    uint8 *Data() { return buffer; }

    const uint8 *DataConst() const { return buffer; }

    uint8 *Last()
    {
        return buffer + Size();
    }

    void Realloc(int _size)
    {
        size = _size;

        if (size > size_buffer)
        {
            LOG_ERROR("%s:%d : Very small buffer %d, need %d", __FILE__, __LINE__, MAX_SIZE, size);
            size = size_buffer;
        }
    }

    void RemoveFirstByte()
    {
        if (size == 0)
        {
            return;
        }
        else if (size == 1)
        {
            size = 0;
        }
        else
        {
            size--;

            std::memmove(buffer, buffer + 1, (uint)size);
        }
    }

    void ReallocAndFill(int _size, uint8 value)
    {
        Realloc(_size);

        for (int i = 0; i < size; i++)
        {
            buffer[i] = value;
        }
    }

    // Записать в буфер size байт из buffer. Если памяти выделено меньше, заполнить только выделенную память
    void FillFromBuffer(const uint8 *in, int _size)
    {
        if (_size > Size())
        {
            LOG_ERROR("%s:%d : Very small buffer %d, need %d", __FILE__, __LINE__, MAX_SIZE, _size);
            _size = Size();
        }

        for (int i = 0; i < _size; i++)
        {
            buffer[i] = in[i];
        }
    }

    // Возвращает количество элементов в буфере
    int Size() const
    {
        return size;
    }

    int Capacity() const
    {
        return size_buffer;
    }

    void Push(uint8 byte)
    {
        Append(&byte, 1);
    }

    void Clear()
    {
        size = 0;
    }

    void Append(const void *data, int _size)
    {
        if (Size() + _size > Capacity())
        {
            LOG_ERROR("Нет места в буфере");
        }
        else
        {
            std::memcpy(&buffer[size], data, (uint)_size);
            size += _size;
        }
    }

    uint8 &operator[](uint i)
    {
        if ((int)i >= 0 && (int)i < Size())
        {
            return buffer[i];
        }

        static uint8 null(0);

        return null;
    }

    uint8 &operator[](int i)
    {
        if (i >= 0 && i < Size())
        {
            return buffer[i];
        }

        static uint8 null(0);

        return null;
    }

protected:

    int size;

    uint8 buffer[size_buffer];
};
