#pragma once


struct ColorScheme { enum E
{
    WhiteLetters,       // В этом случае заголовки элементов меню пишутся белым - не очень хорошо видно снизу
    BlackLetters        // В этом случае заголовки элементов меню пишутся чёрным - не очень красиво выглядит
};};

// Режим отрисовки сигнала.
struct ModeDrawSignal { enum E
{
    Lines = 0,   // линиями
    Points = 1   // точками
};};

// Тип сетки на экране.
struct TypeGrid { enum E
{
    _1,
    _2,
    _3,
    _4,
    Count
};};

// Количество накоплений.
struct ENumAccumulation { enum E
{
    _1,
    _2,
    _4,
    _8,
    _16,
    _32,
    _64,
    _128,
    _Infinity
};};

// Количество усреднений по измерениям.
struct ENumAveraging { enum E
{
    _1,
    _2,
    _4,
    _8,
    _16,
    _32,
    _64,
    _128,
    _256,
    _512
};};

// Тип усреднений по измерениям
struct ModeAveraging { enum E
{
    Accurately,   // Усреднять точно.
    Around        // Усреднять приблизительно.
}; };

// Количество измерений для расчёта минимального и максимального значений.
struct ENumMinMax { enum E
{
    _1,
    _2,
    _4,
    _8,
    _16,
    _32,
    _64,
    _128
}; };

// Количество точек для расчёта сглаживания.
struct DisplaySmoothing { enum E
{
    Disable,
    _2points,
    _3points,
    _4points,
    _5points,
    _6points,
    _7points,
    _8points,
    _9points,
    _10points
};};

// Ограничение FPS.
struct ENumSignalsInSec { enum E
{
    _25,
    _10,
    _5,
    _2,
    _1
}; };

struct ModeAccumulation { enum E
{
    NoReset,   // В этом режиме показываются строго N последних
    Reset      // В этом режиме набираются N последних и потом сбрасываются
};};

// Режим отображения дополнительных боковых маркеров смещения по напряжению 
struct AltMarkers { enum E
{
    Hide,        // Никода не выводить
    Show,        // Всегда выводить
    AutoHide     // Выводить и прятать через timeShowLevels
};};

// Через какое время после последнего нажатия кнопки скрывать меню
struct MenuAutoHide { enum E
{
    None   = 0,    // Никогда
    _5     = 5,    // Через 5 секунд
    _10    = 10,   // Через 10 секунд
    _15    = 15,   // Через 15 секунд
    _30    = 30,   // Через 30 секунд
    _60    = 60    // Через 60 секунд
};};


// ChannelX

struct Channel { enum E
{
    A,
    B,
    A_B,
    Math
};};

#define NumChannels 2

// Режим канала по входу
struct ModeCouple { enum E
{
    DC,      // Открытый вход
    AC,      // Закрытый вход
    GND      // Вход заземлён
};};

// Масштаб по напряжению
struct Range { enum E
{
    _2mV,
    _5mV,
    _10mV,
    _20mV,
    _50mV,
    _100mV,
    _200mV,
    _500mV,
    _1V,
    _2V,
    _5V,
    _10V,
    _20V,
    Count
};};

// Режим запуска
struct StartMode { enum E
{
    Auto,     // Автоматический
    Wait,     // Ждущий 
    Single    // Однократный
};};

// Источник синхронизации
struct TrigSource { enum E
{
    A,      // Канал 1
    B,      // Канал 2
    Ext     // Внешняя
};};

// Тип синхронизацц
struct TrigPolarity { enum E
{
    Front,     // По фронту
    Back       // По срезу
};};

// Вход синхронизации
struct TrigInput { enum E
{
    Full,         // Полный сиганл
    AC,           // Переменный
    LPF,          // ФНЧ
    HPF           // ФВЧ
};};

struct TrigModeFind { enum E
{
    Hand,          // Уровень синхронизации устанавливается вручную или автоматически - однократным нажажтием кнопки
    Auto           // Подстройки уровня синхронизации производится автоматически после каждого нового считанного сигнала
};};

// Режим длительного нажатия кнопки СИНХР
struct ModeLongPressTrig { enum E
{
    Level0,
    Auto
};};


// Time


// Функция ВР/ДЕЛ
struct FunctionTime { enum E
{
    Time,
    ShiftInMemory
}; };

// Масштаб по времени
struct TBase { enum E
{
    _2ns,
    _5ns,
    _10ns,
    _20ns,
    _50ns,
    _100ns,
    _200ns,
    _500ns,
    _1us, 
    _2us,
    _5us,
    _10us,
    _20us,
    _50us,
    _100us,
    _200us,
    _500us,
    _1ms,
    _2ms,
    _5ms,
    _10ms,
    _20ms,
    _50ms,
    _100ms,
    _200ms,
    _500ms,
    _1s,
    _2s,
    _5s,
    _10s,
    Count
}; };

// Положение точки синхронизация на сигнале
struct TPos { enum E
{
    Left,      // Привязка к левому краю
    Center,    // Привязка к центру
    Right      // Привязка к правому краю
};};

struct PeackDetMode { enum E
{
    Disable,
    Enable,
    Average
};};


// Cursors


// Каким курсором управлять
struct CursCntrl { enum E
{
    _1,        // первым
    _2,        // вторым
    _1_2,      // обоими
    Disable   // никаким
};};

// Дискретность перемещения курсоров
struct CursMovement { enum E
{
    Points,    // по точкам
    Percents   // по процентам
};};

// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
struct CursActive { enum E
{
    U,
    T,
    None
};};

// Режим слежения курсоров
struct CursLookMode { enum E
{
    None,          // Курсоры не следят
    Voltage,       // Курсоры следят за напряжением автоматически
    Time,          // Курсоры следят за временем автоматически
    Both           // Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
};};


// Режим работы.
struct ModeWork { enum E
{
    Direct,     // Основной режим.
    Latest,     // В этом режиме можно просмотреть последние сохранённые измерения
    MemInt,     // В этом режиме можно сохранять во flash-памяти измерения просматривать ранее сохранённые
};};

// Что показывать в режиме Внутр ЗУ - считанный или записанный сигнал
struct ModeShowIntMem { enum E
{
    Direct,     // Показывать данные реального времени.
    Saved,      // Показывать сохранённые данные
    Both        // Показывать данные реального времени и сохранённые данные
};};

// Что делать при нажатии кнопки ПАМЯТЬ
struct ModeBtnMemory { enum E
{
    Menu,       // Будет открывааться соответствующая страница меню
    Save        // Сохранение содержимого экрана на флешку
};};

// Режим наименования файлов
struct FileNamingMode { enum E
{
    Mask,        // Именовать по маске
    Manually     // Именовать вручную
};};

// Как сохранять данные на флешку
struct ModeSaveSignal { enum E
{
    BMP,
    TXT
};};


// Measures


// Сжимать ли сигналы при выводе измерений.
struct ModeViewSignals { enum E
{
    AsIs,       // Показывать сигналы как есть
    Compress    // Сжимать сетку с сигналами
};};

struct MeasuresNumber { enum E
{
    _1,             // 1 измерение слева внизу
    _2,             // 2 измерения слева внизу
    _1_5,           // 1 строка с 5 измерениями
    _2_5,           // 2 строки по 5 измерений
    _3_5,           // 3 строки по 5 измерений
    _6_1,           // 6 строк по 1 измерению
    _6_2            // 6 строк по 2 измерения
};};

// Зона, по которой считаются измрения
struct MeasuresField { enum E
{
    Sfcreen,        // Измерения будут производиться по той части сингала, которая видна на экране.
    AllMemory,      // Измерения будут производиться по всему сигналу.
    Hand            // Измерения будут производиться по окну, задаваемому пользователем.
};};


struct Function { enum E
{
    Sum,
    Mul
};};

struct ScaleFFT { enum E
{
    Log,           // Это значение означает логарифмическую шкалу вывода спектра
    Linear         // Это значение означает линейную шкалу вывода спектра
}; };

struct WindowFFT { enum E
{
    Rectangle,
    Hamming,
    Blackman,
    Hann
};};

struct FFTmaxDB { enum E
{
    _40,
    _60,
    _80
};};

struct ModeDrawMath { enum E
{
    Disable,
    Separate,
    Together
};};

struct ModeRegSet { enum E
{
    Range,
    RShift
};};


struct CalibratorMode { enum E
{
    Freq,
    DC,
    GND
};};


// Debug


// Тип балансировки АЦП каналов.
// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение для одного из АЦП канала.
struct BalanceADCtype { enum E
{
    Disable,     // Балансировка выключена.
    Settings,    // Используются значения балансировки, которые получены автоматически.
    Hand         // Используются значения балансировки, заданные вручную.
};};

// Тип растяжки АЦП
struct StretchADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};

// Тип дополнительного смещения
enum RShiftADCtype
{
    RShiftADC_Disable,
    RShiftADC_Settings,
    RShiftADC_Hand
};
