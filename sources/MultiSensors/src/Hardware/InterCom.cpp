// Sasha7b9@tut.by (c)
#include "defines.h"
#include "Hardware/InterCom.h"
#include "Modules/ST7735/ST7735.h"
#include "Display/Display.h"
#include "Utils/Text/String.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Storage/Measures.h"

#ifdef GUI
    #include "Hardware/LAN/ClientTCP.h"
#endif


namespace InterCom
{
    /*
    *  Формат сообщения.
    *  0       - 'A'
    *  1       - 'B'
    *  2       - 'C'
    *  3       - type
    *  4 ...11 - ID
    *  12...15 - hash[12...15]
    *  16...19 - value
    */

    static Direction::E direction = Direction::_None;
}


void InterCom::SetDirection(Direction::E dir)
{
    direction = dir;
}


void InterCom::Send(const Measure &measure, uint timeMS)
{
    static const pchar names[Measure::Count] =
    {
        "Temperature",
        "Pressure",
        "Humidity",
        "DewPoint",
        "Velocity",
        "Latitude",
        "Longitude",
        "Altitude",
        "Azimuth",
        "Illuminate",
        "Distance"
    };

    static const pchar units[Measure::Count] =
    {
        "degress Celsius",
        "hPa",
        "%%",
        "degress Celsius",
        "m/s",
        "degress",
        "degress",
        "m",
        "degress",
        "lxs",
        "m"
    };

    if (direction & Direction::Display)
    {
        if (!Measures::IsFixed())
        {
            Display::SetMeasure(measure, timeMS);
        }
    }

    if (direction & Direction::CDC)
    {
        String<> message("%s : %f %s", names[measure.GetName()], measure.GetDouble(), units[measure.GetName()]);

//        HCDC::Transmit(message.c_str(), message.Size() + 1);
    }

#ifdef GUI

    ClientTCP::Transmit(data.Data(), data.Size()); 

#endif
}
