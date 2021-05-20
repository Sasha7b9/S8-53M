// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


class Panel
{
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

    // Включить лампочку
    static void EnableLED(TypeLED::E led);

    // Выключить лампочку
    static void DisableLED(TypeLED::E led);

    // Переключить лампочку в соответствующее состояние
    static void SwitchToStateLED(TypeLED::E led, bool enabled);

    // Эта функция должна вызываться из приёмной фунции SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);

    // Ожидать нажатие клавиши.
    static Key::E WaitPressingButton();

   static uint8 NextData();

private:

    static bool isRunning;

    // Передать даннные в мк панели управления.
    static void TransmitData(uint8 data);
};
