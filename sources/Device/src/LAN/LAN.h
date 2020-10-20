#pragma once


class LAN
{
public:

    static void Init();
    // Функция будет обрабатывать входной/выходной буфер timeMS миллисекунд. Это надо для передчи больших массивов информации. WARN говнокод. Надо исправить.
    static void Update(uint timeMS);

    static bool cableLANisConnected;    // Если 1, значит, просто подключён кабель
};
