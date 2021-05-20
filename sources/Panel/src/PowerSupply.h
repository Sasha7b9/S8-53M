// 2021/05/05 15:03:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class PowerSupply
{
public:

    // ���������� true ������ � ��� ������, ���� � ������ ������ ��������� ���������
    static bool AttemptToTurnOn(KeyboardEvent &event);

    static void TurnOff();

    static bool IsEnabled();

private:

    static uint timeEnabled;    // ����� ���������. ���� (-1), �� ���������

    static uint timeDisable;    // ����� ���������� ����������
};
