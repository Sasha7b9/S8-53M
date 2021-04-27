// 2021/04/27 11:49:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/Sector_.h"


const Sector &Sector::Get(Sector::E number)
{
    return HAL_ROM::sectors[number];
}
