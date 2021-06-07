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
        StretchADCtype::E type_stretch;
        RShiftADCtype::E  type_rshift;
        int16             first_byte;   // Не используется. Оставлено для быстрого включения соотвествующей настройки
    };

    struct SettingsChannel
    {
        int16 balance_ADC;                      // Баланс АЦП
        int16 rshift[Range::Count][2];          // Добавочное смещение, которое пишется сюда при калибровке
                                                // и балансировке. При z == 0 - ModeCouple::DC, при z == 1
                                                // - ModeCouple::AC
                                                // Range в 3-х первых индексах при ModeCouple::DC задаются вручную
        int16 stretch_hand;                     // Значение растяжки канала для ручного режима
        float stretch_auto;                     // Растяжка, рассчитанная в процессе калибровки

        int16 AddRShift(Range::E range, ModeCouple::E mode);
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
    static void ExecuteOnChanged();
    static void ExecuteOnChanged(bool);

    bool operator!=(const SettingsNRST &rhs);
};


extern SettingsNRST setNRST;
