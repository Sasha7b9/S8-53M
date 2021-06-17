#pragma once


class LAN
{
public:

    // Инициализация
    static void Init();

    // Функция будет обрабатывать входной/выходной буфер timeMS миллисекунд. Это надо для передчи больших массивов информации.
    // \todo говнокод. Надо исправить.
    static void Update(uint timeMS);

    static bool clientIsConnected;

    static bool cableIsConnected;
};
