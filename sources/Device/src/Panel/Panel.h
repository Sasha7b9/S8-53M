// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


class Panel
{
public:

    static void Init();

    static void Update();

    // � ���������� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� �������� ������ ������� ��������
    static void Disable();

    static void EnableLED(TypeLED::E led, bool endable);

    // ��� ������� ������ ���������� �� ������� ������ SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    // ������� ������� �������.
    static Key::E WaitPressingButton();

    static void Enable();

    // �������� ������� � �� ������ ����������.
    static void TransmitData(uint8 data);

    static uint8 NextData();

private:

    static bool isRunning;
};
