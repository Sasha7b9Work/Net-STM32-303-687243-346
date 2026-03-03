// 2026/03/03 12:14:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Record.h"
#include "Utils/Math.h"


uint Record::CalculateCRC() const
{
    return Math::CalculateCRC(BeginData(), SizeData());
}


bool Record::IsValid() const
{
    return  crc == CalculateCRC();
}


int Record::Size() const
{
    return sizeof(*this);
}
