// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


struct LED
{
    enum E
    {
        // ���/���� ������������ ���: ���� ����, �� ���������� ��� ��������, ���� ��������, �� ��������. ������� ���
        _None,
        Trig,   // 1 / 128 + 1
        RegSet, // 2 / 128 + 2
        ChanA,  // 3 / 128 + 3
        ChanB,  // 4 / 128 + 4
        Power
    };
};


class Panel
{
public:

    static void Init();

    static void Update();

    // � ���������� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� �������� ������ ������� ��������.
    static void Disable();

    static void EnableLED(LED::E led, bool endable);

    // ��� ������� ������ ���������� �� ������� ������ SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    // ������� ������� �������.
    static Key::E WaitPressingButton();

    static void Enable();

    // �������� ������� � �� ������ ����������.
    static void TransmitData(uint8 data);

private:

    static bool isRunning;
};
