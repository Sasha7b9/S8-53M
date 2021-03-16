// 2021/03/16 10:47:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Mutex_.h"


void Mutex::Lock()
{
    locked = true;
}


void Mutex::Unlock()
{
    locked = false;
}


bool Mutex::IsLocked() const
{
    return locked;
}
