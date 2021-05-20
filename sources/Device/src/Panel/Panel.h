// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


class Panel
{
    friend struct LED;

public:

    static void Init();

    static void Update();

    // Отключить обработку ввода. В этом режиме панель лишь обновляет состояние переменной pressedButton, не выполняя
    // больше никаких действий
    static void DisableInput();

    // Включить обработку ввода
    static void EnableInput();

    // Послать сигнал выключения
    static void DisablePower();

    // Ожидать нажатие клавиши.
    static Key::E WaitPressingButton();

public:

    // Эта функция должна вызываться из приёмной фунции SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    // Эта функция вызывается из передающей функции SPI5
    static uint8 NextData();

private:

    static bool isRunning;

    // Передать даннные в мк панели управления.
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
    // Возвращает true, если лампочка УСТАНОВКА должна гореть
    bool NeedForFire();
};


extern LED       led_Trig;
extern RegSetLED led_RegSet;
extern LED       led_ChA;
extern LED       led_ChB;
extern LED       led_Power;
