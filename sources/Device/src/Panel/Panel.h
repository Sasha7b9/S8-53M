// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


class Panel
{
public:

    static void Init();

    static void Update();

    // В отлюченном режиме панель лишь обновляет состояние переменной pressedButton, не выполняя больше никаких действий
    static void Disable();

    static void EnableLED(TypeLED::E led, bool endable);

    // Эта функция должна вызываться из приёмной фунции SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    // Ожидать нажатие клавиши.
    static Key::E WaitPressingButton();

    static void Enable();

    // Передать даннные в мк панели управления.
    static void TransmitData(uint8 data);

    static uint8 NextData();

private:

    static bool isRunning;
};
