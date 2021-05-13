// 2021/05/12 10:18:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"


struct OutputRegisters
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


struct SettingsNRST
{
    int16             num_srings;                 // Число строк в консоли.
    int8              size_font;                  // Размер шрифта консоли - 0 - 5, 1 - 8,
    bool              console_in_pause;           // \brief Признак того, что консоль находится в режиме паузы. Режим
                                                  // паузы означает, что новые 
                                                  // сообщения она не записывает и не сохраняет.
    BalanceADCtype::E balance_ADC_type;           // Тип балансировки.
    int16             balance_ADC[2];             // Значение дополнительного смещения АЦП для ручной балансировки.
    StretchADCtype::E stretch_ADC_type;           // Тип растяжки канала.
    int16             stretch_ADC[2];             // Значение растяжки канала для ручного режима.
    int16             num_measures_for_gates;     // Число измерений для ворот.
    int16             shift_T0;                   // Дополнительное смещение по времени для данной развёртки режима
                                                  // рандомизатора.
    bool              show_stats;                 // Показывать статистику на экране (fps, например).
    int16             num_ave_for_rand;           // По скольким измерениям усреднять сигнал в режиме рандомизатора.
    bool              viewAlteraWrittingData;     // Показывать ли данные, идущие в альтеру.
    bool              viewAllAlteraWrittingData;  // \brief Показывать ли все данные, идущие в альтеру (если false, то
                                                  // постоянно идущие команды вроде 
                                                  // START, STOP не показываются).
    int16             altShift;                   // Добавочное смещение для устранения эффекта горизонтальной помехи
                                                  // синхронизации.
    int16             num_smooth_for_rand;        // Число точек для скользящего фильтра в рандомизаторе
    OutputRegisters   show_registers;

    static int  GetSizeFontForConsole();        // Возвращает размер шрифта, которым нужно выводить сообщения в консоли.
};


extern SettingsNRST setNRST;
