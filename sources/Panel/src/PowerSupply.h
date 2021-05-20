// 2021/05/05 15:03:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class PowerSupply
{
public:

    static void AttemptToTurnOn(KeyboardEvent &event);

    static void TurnOff();

    static bool IsEnabled();

private:

    static uint timeEnabled;    // Время включения. Если (-1), то выключено
};
