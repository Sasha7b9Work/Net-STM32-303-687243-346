// 2022/05/05 15:23:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"


static const Settings def_set =
{
    0,
    0,
    // Display
    {
        {1, 1, 1, 0},
        { TypeDisplayedInformation::AllMeasures }
    },
    // System
    {
        0                                           // serial_number
    },
    // Measures
    {//  Temperature         Pressure           Humidity          DewPoi t            Velocity
        { 18,                0,                 0,                 -30,               0},
        { 99,                1000,              100,               99,                1},
        { ERROR_VALUE_FLOAT, ERROR_VALUE_FLOAT, ERROR_VALUE_FLOAT, ERROR_VALUE_FLOAT, ERROR_VALUE_FLOAT },
        { ERROR_VALUE_FLOAT, ERROR_VALUE_FLOAT, ERROR_VALUE_FLOAT, ERROR_VALUE_FLOAT, ERROR_VALUE_FLOAT }
    }
};


Settings gset = def_set;


//void Settings::Load(Settings *set)
//{
//#ifndef GUI
//
//    Settings settings;
//
//    if (HAL_ROM::LoadSettings(&settings))
//    {
//        *set = settings;
//    }
//    else
//    {
//        *set = def_set;
//    }
//
//#endif
//}


//void Settings::Reset()
//{
//    gset = def_set;
//}


//void Settings::Save(Settings *set)
//{
//#ifndef GUI
//
//    Settings settings;
//
//    if (HAL_ROM::LoadSettings(&settings))
//    {
//        if (*set == settings)
//        {
//            return;
//        }
//    }
//
//    gset.number++;
//
//    set->number = gset.number;
//
//    HAL_ROM::SaveSettings(set);
//
//#endif
//}


bool Settings::operator==(const Settings &rhs)
{
    return  std::memcmp(&display, &rhs.display, sizeof(display)) == 0 && //-V1103
            std::memcmp(&system, &rhs.system, sizeof(system)) == 0 &&
            std::memcmp(&measures, &rhs.measures, sizeof(measures)) == 0; //-V1014
}


bool Settings::operator!=(const Settings &rhs)
{
    return !(*this == rhs);
}


void Settings::SaveMeasure(const Measure &measure)
{
    float value = (float)measure.GetDouble();

    Measure::E name = measure.GetName();

    bool need_save = false;
    (void)need_save;

    if (gset.measures.value_max[name] == ERROR_VALUE_FLOAT)
    {
        gset.measures.value_max[name] = value;
        need_save = true;
    }

    if (gset.measures.value_min[name] == ERROR_VALUE_FLOAT)
    {
        gset.measures.value_min[name] = value;
        need_save = true;
    }

    if (value > gset.measures.value_max[name])
    {
        gset.measures.value_max[name] = value;
        need_save = true;
    }
    else if (value < gset.measures.value_min[name])
    {
        gset.measures.value_min[name] = value;
        need_save = true;
    }

//    Settings::Save(&gset);
}


void Settings::ResetMeasure(Measure::E name)
{
    gset.measures.value_max[name] = ERROR_VALUE_FLOAT;

    gset.measures.value_min[name] = ERROR_VALUE_FLOAT;
}
