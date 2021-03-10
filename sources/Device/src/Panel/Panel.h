#pragma once
#include "common/Panel/Controls_.h"


class Panel
{
public:

    static void Init();

    static void Update();

    static void EnableLEDRegSet(bool enable);

    // Включить/выключить светодиод КАНАЛ 1.
    static void EnableLEDChannel0(bool enable);

    // Включить/выключить светодиод КАНАЛ 2.
    static void EnableLEDChannel1(bool enable);

    // Включить/выключить светодиод СИНХР.
    static void EnableLEDTrig(bool enable);

    // Передать даннные в мк панели управления.
    static void TransmitData(uint16 data);

    // В отлюченном режиме панель лишь обновляет состояние переменной pressedButton, не выполняя больше никаких действий.
    static void Disable();

    static void Enable();

    // Ожидать нажатие клавиши.
    static Key::E WaitPressingButton();

    // Эта функция должна вызываться из приёмной фунции SPI5
    static void CallbackOnReceiveSPI5(uint8 *data, int size);

    static uint16 NextData();

    static bool isRunning;

    static void Draw();
};
