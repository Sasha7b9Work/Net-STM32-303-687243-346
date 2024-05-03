#pragma once


struct tm
{
    int tm_sec;     // 0-61
    int tm_min;     // 0-59
    int tm_hour;    // 0-23
    int tm_mday;    // 1-31
    int tm_mon;     // 0-11
    int tm_year;    // years since 1900
    int tm_wday;    // days since Sunday        0-6
    int tm_yday;    // days since January 1     0-365
    int tm_isdst;   // Daylight Saving Time flag
};


typedef unsigned long long time_t;

time_t mktime(struct tm *);

struct tm * gmtime (const time_t * timer);
