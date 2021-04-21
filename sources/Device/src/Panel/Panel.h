// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


class Panel
{
public:

    static void Init();

    static void Update();

    static void EnableLEDRegSet(bool enable);

    // ��������/��������� ��������� ����� 1.
    static void EnableLEDChannel0(bool enable);

    // ��������/��������� ��������� ����� 2.
    static void EnableLEDChannel1(bool enable);

    // ��������/��������� ��������� �����.
    static void EnableLEDTrig(bool enable);

    // �������� ������� � �� ������ ����������.
    static void TransmitData(uint16 data);

    // � ���������� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� �������� ������ ������� ��������.
    static void Disable();

    static void Enable();

    // ������� ������� �������.
    static Key::E WaitPressingButton();

    // ��� ������� ������ ���������� �� ������� ������ SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    static uint16 NextData();

    static bool isRunning;
};
