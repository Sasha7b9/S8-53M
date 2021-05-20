// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


class Panel
{
    friend struct LED;

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

    // ������� ������� �������.
    static Key::E WaitPressingButton();

public:

    // ��� ������� ������ ���������� �� ������� ������ SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    // ��� ������� ���������� �� ���������� ������� SPI5
    static uint8 NextData();

private:

    static bool isRunning;

    // �������� ������� � �� ������ ����������.
    static void TransmitData(uint8 data);
};


struct LED
{
    LED(TypeLED::E led);
    void Disable();
    void SwitchToState(bool enabled);
private:
    TypeLED::E led;
};


struct RegSetLED : public LED
{
    RegSetLED(TypeLED::E led) : LED(led) {}
    void Switch();
private:
    // ���������� true, ���� �������� ��������� ������ ������
    bool NeedForFire();
};


extern LED       led_Trig;
extern RegSetLED led_RegSet;
extern LED       led_ChA;
extern LED       led_ChB;
extern LED       led_Power;
