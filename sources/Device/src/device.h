#pragma once


class Device
{
public:
    static void Init();
    static void Update();
    static bool needResetSettings;  // ���� 1, �� ��������� ����� ��������
};
