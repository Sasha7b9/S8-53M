// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class LAN
{
public:

    static void Init();
    // ������� ����� ������������ �������/�������� ����� timeMS �����������. ��� ���� ��� ������� ������� �������� ����������. WARN ��������. ���� ���������.
    static void Update(uint timeMS);

    static bool cableIsConnected;       // ���� 1, ������, ������ ��������� ������
    static bool clientIsConnected;      // ���� 1, �� ���������� ������
};
