// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


class Interface
{
public:

    static void Update(KeyboardEvent &event);

private:

    static void ProcessByte(uint8 byte);
};
