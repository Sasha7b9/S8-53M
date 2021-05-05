// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


struct LED
{
    enum E
    {
        Trig,
        RegSet,
        ChanA,
        ChanB
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

//    // ��������/��������� ��������� �����.
//    static void EnableLEDTrig(bool enable);
//
//    static void EnableLEDRegSet(bool enable);
//
//    // ��������/��������� ��������� ����� 1.
//    static void EnableLEDChannel0(bool enable);
//
//    // ��������/��������� ��������� ����� 2.
//    static void EnableLEDChannel1(bool enable);

    // ��� ������� ������ ���������� �� ������� ������ SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    // ������� ������� �������.
    static Key::E WaitPressingButton();

    static void Enable();

    // �������� ������� � �� ������ ����������.
    static void TransmitData(uint16 data);

private:

    // �������� ��������� ������ ��� ��������
    static uint16 NextDataForTransmit();

    static bool isRunning;
};
