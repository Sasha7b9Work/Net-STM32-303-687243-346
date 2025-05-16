// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"

/*
*   Каждое изделие обладает уникальным ID, который вводится после прошивки через меню
*/

#ifndef WIN32
    #if __ARMCC_VERSION != 6210000
        // На других версиях компиляторов не проверялось
        // Но на 6.23 (и, видимо, 6.22) из Keil 5.42a вылетает с BKPT 0xAB. Нужна реализация putc, etc for _sys_open (файл Retarget.c)
        #error "Requires ARM Compiler V6.21 from uVision 5.39"
    #endif
#endif

int main(void)
{
    Device::Init();

    while (true)
    {
        Device::Update();
    }
}
