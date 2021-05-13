// 2021/05/12 10:18:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"


struct SettingsNRST
{
    struct SettingsRangomizer
    {
        int16   num_ave;                // По скольким измерениям усреднять сигнал
        int16   num_smooth;             // Число точек для скользящего фильтра
        int16   num_measures_for_gates; // Число измерений для ворот
        int16   shift_T0;               // Дополнительное смещение по времени для данной развёртки
        bool    show_info;              // Выводить ворота и считанное значение
    };

    struct SettingsADC
    {
        BalanceADCtype::E type_balance; // Тип балансировки
        StretchADCtype::E type_stretch; // Тип растяжки канала.
    };

    struct SettingsChannel
    {
        int16   balance_hand;                // Значение дополнительного смещения АЦП для ручной балансировки
        int8    balance_auto;                // Дополнительное смещения АЦП, рассчитанное в процессе калибровки
        int16   stretch_hand;                // Значение растяжки канала для ручного режима
        float   stretch_auto;                // Растяжка, рассчитанная в процессе калибровки
        int16   rshift_add[Range::Count][2]; // Добавочное смещение для открытого (0) и закрытого (1) входов
    };

    struct SettingsConsole
    {
        int16   num_srings;     // Число строк в консоли.
        int8    size_font;      // Размер шрифта консоли - 0 - 5, 1 - 8,
        bool    in_pause;       // Признак того, что консоль находится в режиме паузы. Режим паузы означает, что новые 
                                // сообщения она не записывает и не сохраняет.

        // Возвращает размер шрифта, которым нужно выводить сообщения в консоли.
        static int  GetSizeFontForConsole();
    };

    struct SettingsRegisters
    {
        bool launch;
        bool flag;           // Выводить ли флаг готовности.
        bool rShiftA;
        bool rShiftB;
        bool trigLev;
        bool rangeA;
        bool rangeB;
        bool chanParamA;
        bool chanParamB;
        bool trigParam;
        bool tShift;
        bool tBase;
    };

    bool               show_stats;                 // Показывать статистику на экране (fps, например).
    SettingsRangomizer rand;
    SettingsADC        adc;
    SettingsChannel    chan[Channel::Count];
    SettingsConsole    console;
    SettingsRegisters  show_registers;

    // Эту функцию нужно вызывать после каждого изменения несбрасываемой настройки
    static void CommonOnChanged();

    static void Save();
};


extern SettingsNRST setNRST;
