// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Хорошо использовать для трассировке, если потом в HardFault_Handler() отслеживать эти переменные
//#define START_PROFILING_US Debug::StartProfilingUS()
//#define POINT_PROFILING_US Debug::PointProfilingUS(__FILE__, __LINE__)

//#define START_PROFILING_MS Debug::StartProfilingMS()
//#define POINT_PROFILING_MS Debug::PointProfilingMS(__FILE__, __LINE__)

//#define DEBUG_POINT(x) Debug::line[x] = __LINE__; Debug::file[x] = __FILE__;
//#define DEBUG_POINT_0  DEBUG_POINT(0)

namespace Debug
{
    void StartProfilingMS();
    uint PointProfilingMS(pchar file, int line);

    extern int line[10];
    extern pchar file[10];
    extern int type;
}
