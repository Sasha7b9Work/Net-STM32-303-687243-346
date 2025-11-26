// 2022/05/05 15:22:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Display.h"


struct TypeDisplayedInformation
{
    enum E
    {
        Temperature,
        Pressure,
        Humidity,
        DewPoint,
        Velocity,
        AllMeasures,
        Count
    };

    E value;

    TypeDisplayedInformation &operator++()
    {
        value = (E)(value + 1);

        return *this;
    }

    bool IsAllMeasures() const { return value == AllMeasures; }
};


struct SettingsDisplay
{
    uint8                       show_measure[Measure::Count];
    TypeDisplayedInformation    typeDisplaydInfo;
};


struct SettingsMeasures
{
    int   limit_min[NUM_MEASURES_TO_CONTROL];
    int   limit_max[NUM_MEASURES_TO_CONTROL];
    float value_min[NUM_MEASURES_TO_CONTROL];
    float value_max[NUM_MEASURES_TO_CONTROL];
};


struct SettingsSystem
{
    int serial_number;
};


struct Settings
{
    uint crc;
    uint number;
    SettingsDisplay  display;
    SettingsSystem   system;
    SettingsMeasures measures;

    bool operator!=(const Settings &);
    bool operator==(const Settings &);

//    static void Load(Settings *set);

//    static void Save(Settings *set);

//    static void Reset();

    // ≈сли value больше или меньше уже сохранЄнного измерени€, то перезаписывает старое
    static void SaveMeasure(const Measure &);

    // —брасывает мин и макс значени€ измерени€
    static void ResetMeasure(Measure::E);
};


extern Settings gset;
