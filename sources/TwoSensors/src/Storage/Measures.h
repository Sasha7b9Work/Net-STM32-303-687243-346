// 2023/06/12 16:46:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
//#include "Display/Display.h"


struct Measure
{
    enum E
    {
        Temperature,    // Температура
        Pressure,       // Давление
        Humidity,       // Влажность
        DewPoint,       // Точка росы
        Velocity,       // Скорость воздуха
        Latitude,       // Широта
        Longitude,      // Долгота
        Altitude,       // Высота
        Azimuth,        // Азимут - угол от направления на север
        Illuminate,     // Освещённость
        Distance,       // Дистанция
        Count
    };

    double value;

    // У первых NumAlarmed() измерений нужно контролировать выход за границы
    static int NumAlarmed() { return 5; }

    void Clear()
    {
        name = E::Count;
        correct = false;
        value = (double)ERROR_VALUE_FLOAT;
    }

    double GetDouble() const
    {
        return value;
    }

    E GetName() const
    {
        return name;
    }

    void Set(E _name, float _value)
    {
        Set(_name, (double)_value);
    }

    void Set(E _name, double _value)
    {
        name = _name;
        SetDouble(_value);
        correct = true;
    }

    bool InRange() const;

    bool correct;

private:

    void SetDouble(float _value)
    {
        SetDouble((double)_value);
    }

    void SetDouble(double _value)
    {
        value = _value;
    }

    E name;
};


namespace Measures
{
    // Фиксирует последние измерения на экране
    void SetFixed(bool fixed);
    bool IsFixed();
}
