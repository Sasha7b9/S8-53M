#pragma once
#include "common/Keyboard/Controls_.h"


class Panel
{
public:

    static void Init();

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

    static bool ProcessingCommandFromPIC(uint16 command);

    static uint16 NextData();

    static bool isRunning;

    // ��� ������� ������ ���������� �� ������� ������ SPI5
    static void CallbackOnReciveSPI5(uint8 *data, int size);
};
