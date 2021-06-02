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
        int16   gate_min;
        int16   gate_max;
    };

    struct SettingsADC
    {
        StretchADCtype::E type_stretch; // Тип растяжки канала.
    };

    struct SettingsChannel
    {
        int16 balance_ADC;                      // Баланс АЦП
        int16 rshift_hand[3];                   // Добавочное смещение, устанавливаемое в ручном режиме для
                                                // ModeCouple::DC и трёх самых чувствительных диапазонах
        int16 rshift_auto[Range::Count][2];     // Добавочное смещение, которое пишется сюда при калибровке
                                                // и балансировке. При z == 0 - ModeCouple::DC, при z == 1
                                                // - ModeCouple::AC
        int16 stretch_hand;                     // Значение растяжки канала для ручного режима
        float stretch_auto;                     // Растяжка, рассчитанная в процессе калибровки
        int16 rshift_add[Range::Count][2];      // Добавочное смещение для открытого (0) и закрытого (1) входов

        // Сохранить дополнительные смещения и сбросить в ноль
        void StoreAndResetRShifts(int16 shifts[3]);

        // Восстановить необходимые дополнительные смещения (те, которые устанавливаются вручную для чувствительных
        // диапазонов
        void RestoreRShifts(int16 shifts[3]);
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

    uint               size;                // Размер структуры.За одно при загрузке из памяти можно определить, что
                                            // структура пуста - здесь будет значение 0xFFFFFFFF
    uint               crc32;               // Контрольная сумма для проверки корректности сохранённых данных
    uint               number;
    bool               show_stats;          // Показывать статистику на экране (fps, например).
    SettingsRangomizer rand;
    SettingsADC        adc;
    SettingsChannel    chan[Channel::Count];
    SettingsConsole    console;
    SettingsRegisters  show_registers;
    CalibrationMode::E calibration_mode[Channel::Count];
    uint               unused_crc32;        // Неиспользуемая переменная. Нужна для расчёта crc32. Всегда равна нулю

    // Инициализация при включении. Проиходит определние наличия настроек в ROM и их загрузка в случае, если настройки
    // есть. Если настроек нету - инициализация значениями по умолчанию
    void Load();

    // Сохранение настроек в ROM. Происходит только если настройки в setNRST не совпадают с хранящимися в ROM
    void Save();

    void Test();

    // Эту функцию нужно вызывать после каждого изменения несбрасываемой настройки
    static void CommonOnChanged();
    static void CommonOnChanged(bool);

    bool operator!=(const SettingsNRST &rhs);
};


extern SettingsNRST setNRST;
