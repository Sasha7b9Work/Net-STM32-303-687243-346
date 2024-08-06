#pragma once


#define LIMITATION(var, value, min, max)    var = (value); if(var < (min)) { var = (min); } else if(var > (max)) { var = (max); };


namespace Math
{
    const float PI = 3.1415296f; //-V624

    uint CalculateCRC(const void *buffer, int size);

    template<class T>
    void CircleIncrease(T *value, T min, T max);

    template<class T>
    void CircleDecrease(T *value, T min, T max);

    inline int Sign(int val)
    {
        return (val < 0) ? -1 : 1;
    }

    float Sin(float angle_rad);

    float Cos(float angle_rad);
}
