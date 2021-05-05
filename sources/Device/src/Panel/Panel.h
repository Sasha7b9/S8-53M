// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


struct LED
{
    enum E
    {
        // Вкл/выкл определяется так: если выкл, то посылается код лампочки, если включить, то устанавл. старший бит
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

    // В отлюченном режиме панель лишь обновляет состояние переменной pressedButton, не выполняя больше никаких действий.
    static void Disable();

    static void EnableLED(LED::E led, bool endable);

    // Эта функция должна вызываться из приёмной фунции SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    // Ожидать нажатие клавиши.
    static Key::E WaitPressingButton();

    static void Enable();

    // Передать даннные в мк панели управления.
    static void TransmitData(uint8 data);

private:

    static bool isRunning;
};
