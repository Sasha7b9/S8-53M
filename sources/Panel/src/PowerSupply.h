// 2021/05/05 15:03:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class PowerSupply
{
public:

    // Возвращает true только в том случае, если в момент вызова произошло включение
    static bool AttemptToTurnOn(KeyboardEvent &event);

    static void TurnOff();

    static bool IsEnabled();

private:

    static uint timeEnabled;    // Время включения. Если (-1), то выключено

    static uint timeDisable;    // Время последнего выключения
};
