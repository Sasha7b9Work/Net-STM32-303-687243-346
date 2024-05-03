// 2022/05/04 11:34:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Math.h"
#include <cmath>


template void Math::CircleIncrease<int>(int *, int, int);
template void Math::CircleDecrease<int>(int *, int, int);


namespace Math
{
    static uint SDBMHash(uint hash, uint8 d)
    {
        return d + (hash << 6) + (hash << 16) - hash;
    }
}


uint Math::CalculateCRC(const void *buffer, int size)
{
    uint hash = 0;

    const uint8 *data = (const uint8 *)buffer;

    while (size--)
    {
        hash = SDBMHash(hash, *data++);
    }

    return hash;
}


template <class T>
void Math::CircleIncrease(T *value, T min, T max)
{
    *value = *value + 1;

    if (*value > max)
    {
        *value = min;
    }
}


template <class T>
void Math::CircleDecrease(T *value, T min, T max)
{
    *value = *value - 1;

    if (*value < min)
    {
        *value = max;
    }
}


float Math::Sin(float angle_rad)
{
    const float k = 180.0f / PI;

    float angle = angle_rad * 180.0f / PI;

    static float values[360];
    static bool first = true;
    if (first)
    {
        first = false;

        for (int i = 0; i < 360; i++)
        {
            values[i] = std::sinf((float)i / k);
        }
    }

    while (angle < 0.0f)
    {
        angle += 360.0f;
    }

    while (angle > 360.0f)
    {
        angle -= 360.0f;
    }

    return values[(int)angle];
}


float Math::Cos(float angle_rad)
{
    const float k = 180.0f / PI;

    float angle = angle_rad * k;

    static float values[360];
    static bool first = true;

    if (first)
    {
        first = false;

        for (int i = 0; i < 360; i++)
        {
            values[i] = std::cosf((float)i / k);
        }
    }

    while (angle < 0.0f)
    {
        angle += 360.0f;
    }

    while (angle >= 360.0f)
    {
        angle -= 360.0f;
    }

    return values[(int)angle];
}
