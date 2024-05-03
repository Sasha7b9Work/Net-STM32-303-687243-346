// 2023/06/01 09:44:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Storage/Measures.h"


#define DEF_GOVERNOR_MIN(_name, page_self, _min, _max, type)                                        \
DEF_GOVERNOR(_name, "Предел мин", *page_self, nullptr, nullptr, _min, _max, gset.measures.limit_min[type])

#define DEF_GOVERNOR_MAX(_name, page_self, _min, _max, type)                                        \
DEF_GOVERNOR(_name, "Предел макс", *page_self, nullptr, nullptr, _min, _max, gset.measures.limit_max[type])

#define DEF_STATE_MIN(_name, page_self, type)                                                       \
DEF_STATE(_name, "Значение мин", *page_self, nullptr, nullptr, type, true)

#define DEF_STATE_MAX(_name, page_self, type)                                                       \
DEF_STATE(_name, "Значение макс", *page_self, nullptr, nullptr, type, false)

#define DEF_STATE_MIN_MAX(_name, page_self, type)                                                   \
DEF_STATE_MIN(_name##ValueMin, page_self, type)                                                     \
DEF_STATE_MAX(_name##ValueMax, page_self, type)


extern const DPage pageMain;


//-------------------------------------------------------------------------------------
static void ClosePageTemperature(bool)
{
    PageMeasures::Temperature::self->Close();
}

DEF_BUTTON(bClosePageTemperature,
    "Закрыть",
    *PageMeasures::Temperature::self,
    ClosePageTemperature,
    nullptr,
    nullptr
)

DEF_CHOICE_2(chTemperature,
    "Показывать",
    *PageMeasures::Temperature::self,
    nullptr,
    nullptr,
    gset.display.show_measure[Measure::Count],
    "Нет", "Да"
)


DEF_GOVERNOR_MIN(gTemperatureLimitMin, PageMeasures::Temperature::self, -30, 60, Measure::Count);

DEF_GOVERNOR_MAX(gTemperatureLimitMax, PageMeasures::Temperature::self, -30, 60, Measure::Count);

DEF_STATE_MIN_MAX(sTemperature, PageMeasures::Temperature::self, Measure::Count);


static void OnPress_ResetTemperature(bool)
{
    gset.ResetMeasure(Measure::Count);
}


DEF_BUTTON(bResetTemperature,
    "Сброс мин-макс",
    *PageMeasures::Temperature::self,
    OnPress_ResetTemperature,
    nullptr,
    nullptr
)


DEF_PAGE_7(pageTemperature, //-V1027
    "ТЕМПЕРАТУРА",
    *PageMeasures::self,
    nullptr,
    nullptr,
    chTemperature,
    gTemperatureLimitMin,
    gTemperatureLimitMax,
    sTemperatureValueMin,
    sTemperatureValueMax,
    bResetTemperature,
    bClosePageTemperature
)


//------------------------------------------------------------------------------------
static void ClosePagePressure(bool)
{
    PageMeasures::Pressure::self->Close();
}

DEF_BUTTON(bClosePagePressure,
    "Закрыть",
    *PageMeasures::Pressure::self,
    ClosePagePressure,
    nullptr,
    nullptr
)

DEF_CHOICE_2(chPressure,
    "Показывать",
    *PageMeasures::Pressure::self,
    nullptr,
    nullptr,
    gset.display.show_measure[Measure::Pressure],
    "Нет", "Да"
)

DEF_GOVERNOR_MIN(gPressureMin, PageMeasures::Pressure::self, 225, 1000, Measure::Pressure);

DEF_GOVERNOR_MAX(gPressureMax, PageMeasures::Pressure::self, 225, 1000, Measure::Pressure);

DEF_STATE_MIN_MAX(sPressure, PageMeasures::Pressure::self, Measure::Pressure);


static void OnPress_ResetPressure(bool)
{
    gset.ResetMeasure(Measure::Pressure);
}


DEF_BUTTON(bResetPressure,
    "Сброс мин-макс",
    *PageMeasures::Pressure::self,
    OnPress_ResetPressure,
    nullptr,
    nullptr
)


DEF_PAGE_7(pagePressure, //-V1027
    "ДАВЛЕНИЕ",
    *PageMeasures::self,
    nullptr,
    nullptr,
    chPressure,
    gPressureMin,
    gPressureMax,
    sPressureValueMin,
    sPressureValueMax,
    bResetPressure,
    bClosePagePressure
)


//-------------------------------------------------------------------------------------
static void ClosePageHumidity(bool)
{
    PageMeasures::Humidity::self->Close();
}

DEF_BUTTON(bClosePageHumidity,
    "Закрыть",
    *PageMeasures::Humidity::self,
    ClosePageHumidity,
    nullptr,
    nullptr
)

DEF_CHOICE_2(chHumidity,
    "Показывать",
    *PageMeasures::Humidity::self,
    nullptr,
    nullptr,
    gset.display.show_measure[Measure::Humidity],
    "Нет", "Да"
)

DEF_GOVERNOR_MIN(gHumidityMin, PageMeasures::Humidity::self, 10, 98, Measure::Humidity);

DEF_GOVERNOR_MAX(gHumidityMax, PageMeasures::Humidity::self, 10, 98, Measure::Humidity);

DEF_STATE_MIN_MAX(sHumidity, PageMeasures::Humidity::self, Measure::Humidity);

static void OnPress_ResetHumidity(bool)
{
    gset.ResetMeasure(Measure::Humidity);
}


DEF_BUTTON(bResetHumidity,
    "Сброс мин-макс",
    *PageMeasures::Humidity::self,
    OnPress_ResetHumidity,
    nullptr,
    nullptr
)

DEF_PAGE_7(pageHumidity, //-V1027
    "ВЛАЖНОСТЬ",
    *PageMeasures::self,
    nullptr,
    nullptr,
    chHumidity,
    gHumidityMin,
    gHumidityMax,
    sHumidityValueMin,
    sHumidityValueMax,
    bResetHumidity,
    bClosePageHumidity
)


//------------------------------------------------------------------------------------
static void ClosePageDewPoint(bool)
{
    PageMeasures::DewPoint::self->Close();
}

DEF_BUTTON(bClosePageDewPoint,
    "Закрыть",
    *PageMeasures::DewPoint::self,
    ClosePageDewPoint,
    nullptr,
    nullptr
)

DEF_CHOICE_2(chDewPoint,
    "Показывать",
    *PageMeasures::DewPoint::self,
    nullptr,
    nullptr,
    gset.display.show_measure[Measure::DewPoint],
    "Нет", "Да"
)

DEF_GOVERNOR_MIN(gDewPointMin, PageMeasures::DewPoint::self, -100, 100, Measure::DewPoint);

DEF_GOVERNOR_MAX(gDewPointMax, PageMeasures::DewPoint::self, -100, 100, Measure::DewPoint);

DEF_STATE_MIN_MAX(sDewPoint, PageMeasures::DewPoint::self, Measure::DewPoint);

//static void OnPress_ResetDewPoint(bool)
//{
//    gset.ResetMeasure(Measure::DewPoint);
//}

/*
DEF_BUTTON(bResetDewPoint,
    "Сброс мин-макс",
    *PageMeasures::DewPoint::self,
    OnPress_ResetDewPoint,
    nullptr,
    nullptr
)
*/

DEF_PAGE_7(pageDewPoint, //-V1027
    "ТОЧКА РОСЫ",
    *PageMeasures::self,
    nullptr,
    nullptr,
    chDewPoint,
    gDewPointMin,
    gDewPointMax,
    sDewPointValueMin,
    sDewPointValueMax,
    bResetHumidity,
    bClosePageDewPoint
)


static bool fixed_marked = false;

static void FixateMeasures(bool)
{
    Measures::SetFixed(!Measures::IsFixed());

    fixed_marked = Measures::IsFixed();
}

DEF_BUTTON(bFixateMeasures,
    "Фиксация",
    *PageMeasures::self,
    FixateMeasures,
    nullptr,
    &fixed_marked
)


static void CloseMeasures(bool)
{
    PageMeasures::self->Close();
};

DEF_BUTTON(bCloseMeasures,
    "Закрыть",
    *PageMeasures::self,
    CloseMeasures,
    nullptr,
    nullptr
)

DEF_PAGE_6(pageMeasures, //-V1027
    "ИЗМЕРЕНИЯ",
    pageMain,
    nullptr,
    nullptr,
    pageTemperature,
    pagePressure,
    pageHumidity,
    pageDewPoint,
    bFixateMeasures,
    bCloseMeasures
)

const Page *const PageMeasures::self = (const Page *)&pageMeasures;
const Page *const PageMeasures::Temperature::self = (const Page *)&pageTemperature;
const Page *const PageMeasures::Pressure::self = (const Page *)&pagePressure;
const Page *const PageMeasures::Humidity::self = (const Page *)&pageHumidity;
const Page *const PageMeasures::DewPoint::self = (const Page *)&pageDewPoint;
