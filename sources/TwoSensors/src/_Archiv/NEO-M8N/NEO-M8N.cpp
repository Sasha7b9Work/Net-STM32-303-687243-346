// 2023/04/04 08:29:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/NEO-M8N/NEO-M8N.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/HC12/HC12.h"
#include "Utils/RingBuffer.h"
#include <cstring>
#include <cstdlib>


namespace HAL_USART2
{
    extern char recv_byte;
}


namespace NEO_M8N
{
    namespace Message
    {
        static const int SIZE = 100;
        static char data[SIZE] = { '\0' };

        void Clear()
        {
            std::memset(data, 0, SIZE);
        }

        void Set(char *str)
        {
            if (std::strlen(str) < SIZE)
            {
                std::strcpy(data, str);
            }
        }

        // Извлекает слово после num_field запятой. Запятая после GNGGA имеет номер 1
        char *ExtractField(int num_filed, char buffer[32])
        {
            if (data[0] == '\0')
            {
                buffer[0] = 0;
            }
            else
            {
                int pos = 0;

                while (num_filed > 0)
                {
                    if (data[pos] == ',' || data[pos] == '\0')
                    {
                        num_filed--;
                    }
                    pos++;
                }

                if (data[pos] == ',' || data[pos] == '\0')
                {
                    buffer[0] = 0;
                }
                else
                {
                    int pos_start = pos;

                    while (data[pos] != ',' && data[pos] != '\0')
                    {
                        pos++;
                    }

                    int pos_end = pos;
                    int iterator = 0;

                    for (pos = pos_start; pos < pos_end; pos++)
                    {
                        buffer[iterator++] = data[pos];
                    }

                    buffer[iterator] = '\0';
                }
            }

            return buffer;
        }
    }

    static RingBuffer<1024> in_buffer;

    static Buffer<2048> out_buffer;
}


void NEO_M8N::CallbackOnReceive()
{
    char symbol = HAL_USART2::recv_byte;

    in_buffer.Append((uint8)symbol);

    HAL_UART_Receive_IT((UART_HandleTypeDef *)HAL_USART2::handle, (uint8 *)&HAL_USART2::recv_byte, 1);
}


void NEO_M8N::Update()
{
#ifdef WIN32
    Message::Set(",064656.00, 3402.91778, S, 15101.42545, E, 1, 12, 0.62, 140.8, M, 18.6, M, , *5E");
#endif
    in_buffer.GetData(out_buffer);

    static bool in_mode_receive = false;        // Если true, то находимся в режиме приёма данных. Строка $GNGGA обнаружена

    static const char *request = "$GNGGA";
    static const int len_request = (int)std::strlen(request);

    while (out_buffer.Size())
    {
        char symbol = (char)out_buffer[0];

        out_buffer.RemoveFirstByte();

        if (in_mode_receive)
        {
            static const int SIZE_DATA = 256;
            static char data[SIZE_DATA];
            static int pointer = 0;

            if (pointer == SIZE_DATA)
            {
                pointer = 0;
            }

            if (symbol == 0x0d || symbol == 0x0a)
            {
                data[pointer] = '\0';
                Message::Clear();
                Message::Set(data);
                in_mode_receive = false;
                pointer = 0;
            }
            else
            {
                data[pointer++] = symbol;
            }
        }
        else
        {
            static int ptr = 0;

            if (symbol == request[ptr])
            {
                ptr++;
                if (ptr == len_request)
                {
                    ptr = 0;
                    in_mode_receive = true;
                }
            }
            else
            {
                ptr = 0;
            }
        }
    }
}


bool NEO_M8N::IsReady()
{
    return true;
}


void NEO_M8N::GetMeasures(Measure *latitude, Measure *longitude, Measure *altitude)
{
    char buffer[32];

    Message::ExtractField(2, buffer);

    double lat = std::atof(buffer) / 100.0;
    latitude->Set(Measure::Latitude, lat);
    latitude->correct = lat > 25.0;

    Message::ExtractField(4, buffer);

    double _long = std::atof(buffer) / 100.0;
    longitude->Set(Measure::Longitude, _long);
    longitude->correct = _long > 25.0;

    Message::ExtractField(9, buffer);

    double alt = std::atof(buffer);
    altitude->Set(Measure::Altitude, alt);
    altitude->correct = alt > 260.0;

    Message::Clear();
}
