#pragma once


class LAN
{
public:

    // �������������
    static void Init();

    // ������� ����� ������������ �������/�������� ����� timeMS �����������. ��� ���� ��� ������� ������� �������� ����������.
    // \todo ��������. ���� ���������.
    static void Update(uint timeMS);

    static bool clientIsConnected;

    static bool cableIsConnected;
};
