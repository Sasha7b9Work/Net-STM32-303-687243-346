// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "Debug.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


//static uint timeStart;
//static uint timePrev;
//static uint timeCounter;
//static uint timeStartCounter;

// Время старта профилировщика
volatile static uint timeStartMS = 0;
// Время предыдущей точки профилировщика
volatile static uint timePrevMS = 0;

pchar Debug::file[10] = { 0 };
int   Debug::line[10] = { 0 };
int   Debug::type = 0;


void Debug::StartProfilingMS()
{
    timeStartMS = TIME_MS;
    timePrevMS = TIME_MS; //-V656
}


uint Debug::PointProfilingMS(pchar /*_file*/, int /*_line*/)
{
    uint d = TIME_MS - timePrevMS;
//    LOG_WRITE("%s %d %d", _file, _line, d);
    timePrevMS = TIME_MS;

    return d;
}
