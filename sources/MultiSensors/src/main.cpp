// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"

/*
*   ������ ������� �������� ���������� ID, ������� �������� ����� �������� ����� ����
*/

#ifndef WIN32
    #if __ARMCC_VERSION != 6210000
        // �� ������ ������� ������������ �� �����������
        // �� �� 6.23 (�, ������, 6.22) �� Keil 5.42a �������� � BKPT 0xAB. ����� ���������� putc, etc for _sys_open (���� Retarget.c)
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
