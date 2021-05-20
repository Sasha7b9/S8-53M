// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


class Panel
{
public:

    static void Init();

    static void Update();

    // ��������� ��������� �����. � ���� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� ��������
    // ������ ������� ��������
    static void DisableInput();

    // �������� ��������� �����
    static void EnableInput();

    // ������� ������ ����������
    static void DisablePower();

    // �������� ��������
    static void EnableLED(TypeLED::E led);

    // ��������� ��������
    static void DisableLED(TypeLED::E led);

    // ����������� �������� � ��������������� ���������
    static void SwitchToStateLED(TypeLED::E led, bool enabled);

    // ��� ������� ������ ���������� �� ������� ������ SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    // ������� ������� �������.
    static Key::E WaitPressingButton();

   static uint8 NextData();

private:

    static bool isRunning;

    // �������� ������� � �� ������ ����������.
    static void TransmitData(uint8 data);
};
