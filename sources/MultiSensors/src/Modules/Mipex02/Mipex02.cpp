// 2025/09/18 08:51:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/Mipex02/Mipex02.h"


namespace Mipex02
{
    static bool is_exist = false;
}


bool Mipex02::Init()
{
    return IsExist();
}


bool Mipex02::IsExist()
{
    return is_exist;
}


void Mipex02::Update()
{

}


bool Mipex02::GetMeasure(Measure *)
{
    return false;
}


void Mipex02::CallbackOnReceive(pchar)
{

}
