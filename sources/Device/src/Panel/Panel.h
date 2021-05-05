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

    // В отлюченном режиме панель лишь обновляет состояние переменной pressedButton, не выполняя больше никаких действий.
    static void Disable();

    static void EnableLED(LED::E led, bool endable);

//    // Включить/выключить светодиод СИНХР.
//    static void EnableLEDTrig(bool enable);
//
//    static void EnableLEDRegSet(bool enable);
//
//    // Включить/выключить светодиод КАНАЛ 1.
//    static void EnableLEDChannel0(bool enable);
//
//    // Включить/выключить светодиод КАНАЛ 2.
//    static void EnableLEDChannel1(bool enable);

    // Эта функция должна вызываться из приёмной фунции SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    // Ожидать нажатие клавиши.
    static Key::E WaitPressingButton();

    static void Enable();

    // Передать даннные в мк панели управления.
    static void TransmitData(uint16 data);

private:

    // Получить следующие данные для передачи
    static uint16 NextDataForTransmit();

    static bool isRunning;
};
