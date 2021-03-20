#pragma once
#include "common/Utils/String_.h"


#define NumChannels 2


// Делитель.
struct Divider { enum E
    {
        _1,
        _10
    };
    static int ToAbs(Divider::E divider);
};


// Тип выборки для режима рандомизатора.
struct SampleType { enum E
{
    Real,   // реальное время - в построении участвуют только реально считанные точки, ничего не рассчитывается.
    Equal   // эквивалентная - сигнал строится по последним точкам, полученным от рандомизатора.
};};


// Число точек сигнала, с которым идёт работа.
struct ENUM_POINTS_FPGA
{
    enum E
    {
        _281,
        _512,
        _1024,
        _2048,
        _4096,
        _8192,
        _16384,
        Count
    };

    static uint ToPoints(E);
};


// Источинк сигнала для отображения спектра.
struct SourceFFT { enum E
{
    A_,          // Расчёт и отображение спектра первого канала.
    B_,          // Расчёт и отображение спектра второго канала.
    Both_        // Расчёт и отображение спектров обоих каналов.
};};


// Язык меню
struct Language { enum E
{
    Russian,    // Русский
    English     // Английский
};};


// Тип растяжки АЦП
struct StretchADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};


// Режим отрисовки сигнала.
struct ModeDrawSignal {enum E
{
    Lines,      // Сигнал рисуется линиями.
    Points      // Сигнал рисуется точками.
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


// Цветовая схема
struct ColorScheme { enum E
{
    WhiteLetters,   // В этом случае заголовки элементов меню пишутся белым - не очень хорошо видно снизу.
    BlackLetters    // В этом случае заголовки элементов меню пишутся чёрным - не очень красиво выглядит.
};};


// Перечисление накоплений.
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
    Infinity
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
    };

    static int NumAverages();
};


// Тип усреднений по измерениям.
struct ModeAveraging { enum E
    {
        Accurately,   // Усреднять точно.
        Around        // Усреднять приблизительно.
    };

    // Возвращает режим усреднения
    static ModeAveraging::E Current();
};


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
};};


// Количество точек для расчёта скользящего фильтра.
struct Smoothing { enum E
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
    };

    // Возвращает число точек сглаживающего фильтра (режим ДИСПЛЕЙ - Сглаживание).
    static uint NumPoints();
};


// Ограничение FPS.
struct ENumSignalsInSec { enum E
    {
        _25,
        _10,
        _5,
        _2,
        _1
    };

    // Возвращает ограничение частоты кадров.
    static int NumSignalsInS();
};


// Режим накопления.
struct ModeAccumulation { enum E
{
    NoReset,   // В этом режиме показываются строго N последних измерений.
    Reset      // В этом режиме набираются N последних измерений и потом сбрасываются.
};};


// Режим отображения дополнительных боковых маркеров смещения по напряжению.
struct AltMarkers { enum E
{
    Hide,        // Никода не выводить.
    Show,        // Всегда выводить.
    AutoHide     // Выводить и прятать через timeShowLevels.
};};


// Через какое время после последнего нажатия кнопки скрывать меню.
struct MenuAutoHide { enum E
    {
        None,   // Никогда.
        _5,     // Через 5 секунд.
        _10,    // Через 10 секунд.
        _15,    // Через 15 секунд.
        _30,    // Через 30 секунд.
        _60     // Через 60 секунд.
    };

    // Возвращает время, через которое меню автоматически скрывается, если не было больше нажатий.
    static int Time();
};


// Тип привязки к смещению по вертикали
struct LinkingRShift { enum E
{
    Voltage,      // Смещение привязано к напряжению
    Position      // Смещение привязаоно к позиции на экране
};};


// Количество и расположение на экране результатов измерений.
struct MeasuresNumber { enum E
{
    _1,             // 1 измерение слева внизу.
    _2,             // 2 измерения слева внизу.
    _1_5,           // 1 строка с 5 измерениями.
    _2_5,           // 2 строки по 5 измерений.
    _3_5,           // 3 строки по 5 измерений.
    _6_1,           // 6 строк по 1 измерению.
    _6_2            // 6 строк по 2 измерения.
};};


// Зона, по которой считаются измрения
struct MeasuresField { enum E
{
    Screen,         // Измерения будут производиться по той части сингала, которая видна на экране.
    AllMemory,      // Измерения будут производиться по всему сигналу.
    Hand            // Измерения будут производиться по окну, задаваемому пользователем.
};};


// Режим работы.
struct ModeWork { enum E
{
    Direct,        // Основной режим.
    Latest,        // В этом режиме можно просмотреть последние сохранённые измерения.
    MemInt,        // В этом режиме можно сохранять во flash-памяти измерения просматривать ранее сохранённые.
};};


// Режим наименования файлов.
struct FileNamingMode { enum E
{
    Mask,        // Именовать по маске.
    Manually     // Именовать вручную.
};};


// Что показывать в режиме Внутр ЗУ - считанный или записанный сигнал.
struct ModeShowIntMem { enum E
{
    Direct,  // Показывать данные реального времени.
    Saved,   // Показывать сохранённые данные.
    Both     // Показывать данные реального времени и сохранённые данные.
};};


// Что делать при нажатии кнопки ПАМЯТЬ.
struct ModeBtnMemory { enum E
{
    Menu,     // Будет открывааться соответствующая страница меню.
    Save      // Сохранение содержимого экрана на флешку.
};};


// Как сохранять данные на флешку.
struct ModeSaveSignal { enum E
{
    BMP,     // Сохранять данные на флешку в формате .bmp.
    TXT      // Сохранять данные на флешку в текствовом виде.
};};


// Функция ручки ВРЕМЯ/ДЕЛ.
struct FunctionTime { enum E
{
    Time,              // Ручка управляет смещением по времени.
    ShiftInMemory      // Ручка управляет отображаемым на экране участком памяти.
};};


// Режим работы пикового детектора.
struct PeackDetMode
{
    enum E
    {
        Disable,
        Enable,
        Average
    };

    static void Set(PeackDetMode::E mode);
    static PeackDetMode::E Get();
    static bool IsEnabled();
};


// Привязка синхронизации к памяти.
struct TPos
{
    enum E
    {
        Left,      // Синхронизация привязана к началу памяти.
        Center,    // Синхронизация привязана к центру памяти.
        Right      // Синхронизация привязана к концу памяти.
    };

    // Узнать привязку отсительно уровня синхронизации в точках
    static int InPoints(PeackDetMode::E peakDet, int numPoints, E tPos);
};


struct Channel
{
    enum E
    {
        A,
        B,
        A_B,
        Math
    } value;

    Channel(E v) : value(v) {}
    bool IsEnabled() const;
    bool IsA() const     { return (value == Channel::A); }
    bool IsB() const     { return (value == Channel::B); }
    bool IsMath() const  { return (value == Channel::Math); }
    operator int() const { return (int)value; }
    Color GetColor() const;
};

extern Channel ChA;
extern Channel ChB;

// Режим канала по входу.
struct ModeCouple
{
    enum E
    {
        DC,      // Открытый вход.
        AC,      // Закрытый вход.
        GND      // Вход заземлён.
    };

    static void Set(const Channel &ch, ModeCouple::E mode);

    static ModeCouple::E Get(const Channel &ch);
    static ModeCouple::E GetA();
    static ModeCouple::E GetB();
};


// Масштаб по напряжению.
struct Range
{
    enum E
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
    };

    static void Load(const Channel &ch);

    static void Set(const Channel &ch, Range::E range);

    static Range::E Get(const Channel &ch);
    static Range::E GetA();
    static Range::E GetB();

    static bool Increase(const Channel &ch);

    static bool Decrease(const Channel &ch);

    static pchar Name(Range::E range);

    static pchar ToString(Range::E range, Divider::E multiplier);
};

Range::E &operator++(Range::E &range);
Range::E &operator--(Range::E &range);


struct RShift
{
    static void Set(const Channel &ch, int16 rShift);

    static int16 Get(const Channel &ch);

    static void Load(const Channel &ch);

    static int ToRel(float rshift_abs, Range::E range);

    static String ToString(int16 rShiftRel, Range::E range, Divider::E multiplier);

    static void Draw();

    static bool show_level_A;    // Нужно ли рисовать горизонтальную линию уровня смещения первого канала
    static bool show_level_B;
    static bool draw_markers;

private:

    static void Draw(const Channel &ch);
};


// Режим запуска.
struct StartMode { enum E
{
    Auto,             // Автоматический.
    Wait,             // Ждущий.
    Single            // Однократный.
};};

// Источник синхронизации.
struct TrigSource
{
    enum E
    {
        A,      // Канал 1.
        B,      // Канал 2.
        Ext     // Внешняя.
    };

    static void Set(E source);
};


// Полярность синхронизацц
struct TrigPolarity
{
    enum E
    {
        Front,  // Синхронизация по фронту.
        Back    // Синхронизация по срезу.
    };

    static void Set(E polarity);

    // Переключение полярности - принудетиельный запуск сбора информации
    static void Switch();

    static void Load();

    static uint timeSwitch;     // Время принудительного запуска сбора информации
};


// Вход синхронизации.
struct TrigInput
{
    enum E
    {
        Full,   // Полный сиганл.
        AC,     // Переменный.
        LPF,    // ФНЧ.
        HPF     // ФВЧ.
    };

    static void Set(E input);
};


// Режим длительного нажатия кнопки СИНХР.
struct ModeLongPressTrig { enum E
{
    Level0,   // Сброс уровня синхронизации в 0.
    Auto      // Автоматический поиск синхронизации - уровень устанавливается посередине между максимумом и минимумом.
};};


// Режим установки синхронизации.
struct TrigModeFind { enum E
{
    Hand,   // Уровень синхронизации устанавливается вручную.
    Auto    // Подстройки уровня синхронизации производится автоматически после каждого нового считанного сигнала
};};


struct TShift
{
    static void Set(int tshift);

    static int16 Get();

    static void Load();

    // Установить добавочное смещение по времени для режима рандомизатора. В каждой развёртке это смещение должно быть
    // разное.
    static void SetDelta(int16 shift);

    // Возвращает установленное смещение по времени в текстовом виде, пригодном для вывода на экран
    static String ToString(int16 tshift_rel);

    // Смещение по времени в точках экрана. Т.к. на канал у нас работают два АЦП поочерёдно, это значение отличается от
    // засылаемого в аппаратную часть в два раза
    static int InPoints(PeackDetMode::E peakDet);

    // Минимальное смещение по времени, которое может быть записано в аппаратную часть.
    static int16 Min();

    // Смещение по времени, соответствующее позиции TPos.
    static int16 Zero();

    static const int NULL_VALUE = 1000000;
};


// Масштаб по времени
struct TBase
{
    enum E
    {
        _1ns,
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
    };

    static void Set(TBase::E tbase);

    static TBase::E Get();

    static void Load();

    static void Decrease();

    static void Increase();

    static pchar Name(TBase::E tbase);

    static pchar ToString(TBase::E tbase);
    static pchar ToStringEN(TBase::E tbase);
};


// Каким курсором управлять.
struct CursCntrl { enum E
{
    _1,            // первым.
    _2,            // вторым.
    _1_2,          // обоими.
    Disable        // никаким.
};};


// Дискретность перемещения курсоров.
struct CursMovement { enum E
{
    Points,    // по точкам.
    Percents   // по процентам.
};};


// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
struct CursActive { enum E
{
    U,
    T,
    None
};};


// Режим слежения курсоров.
struct CursLookMode { enum E
{
    None,      // Курсоры не следят.
    Voltage,   // Курсоры следят за напряжением автоматически.
    Time,      // Курсоры следят за временем автоматически.
    Both       // Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
};};


// Сжимать ли сигналы при выводе измерений.
struct ModeViewSignals { enum E
{
    AsIs,       // Показывать сигналы как есть
    Compress    // Сжимать сетку с сигналами
};};


struct ScaleMath { enum E
{
    Channel0,     // Масштаб берётся из канала 1
    Channel1,     // Масштаб берётся из канала 2
    Hand          // Масштаб задаётся вручную.
};};


// Тип балансировки АЦП каналов.
// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение для одного из АЦП канала.
struct BalanceADCtype { enum E
{
    Disable,     // Балансировка выключена.
    Settings,    // Используются значения балансировки, которые получены автоматически.
    Hand         // Используются значения балансировки, заданные вручную.
};};


// Тип дополнительного смещения
struct RShiftADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};


// Масштаб шкалы для отображения спектра.
struct ScaleFFT { enum E
{
    Log,        // Это значение означает логарифмическую шкалу вывода спектра.
    Linear      // Это значение означает линейную шкалу вывода спектра.
};};


// Вид окна для умножения сигнала перед расчётом спектра.
struct WindowFFT { enum E
{
    Rectangle,  // Перед расчётом спектра сигнал не умножается на окно.
    Hamming,    // Перед расчётом спектра сигнал умножается на окно Хэмминга.
    Blackman,   // Перед расчётом спектра сигнал умножается на окно Блэкмана.
    Hann        // Перед расчётом спектра сигнал умножается на окно Ханна.
};};


// Минимальное значение на вертикальной координате графика спектра.
struct FFTmaxDB { enum E
{
    _40,        // Вертикальная координата до -40Дб
    _60,        // Вертикальная координата до -60Дб
    _80         // Вертикальная координата до -80Дб
};};


// Какую функцию рассчитывать.
struct Function { enum E
{
    Sum,        // Сумма двух каналов.
    Mul         // Произведение двух каналов.
};};


// Где отрисовывать математический сигнал.
struct ModeDrawMath { enum E
{
    Disable,    // Отображение математического сигнала выключено.
    Separate,   // Математический сигнал отображается на отдельном экране.
    Together    // Математический сигнал отображается поверх текущего сигнала.
};};


// Чем управляет ручка УСТАНОВКА, когда включена математическая функция.
struct ModeRegSet { enum E
{
    Range,      // Ручка УСТАНОВКА управляет масштабом по напряжению.
    RShift      // Ручка УСТАНОВКА управляет смещением по напряжению.
};};


// Режим работы калибратора.
struct CalibratorMode
{
    enum E
    {
        Freq,           // На выходе калибратора 4В, 1кГц.
        DC,             // На выходе калибратора 4В постоянного напряжения.
        GND             // На выходе калибратора 0В.
    };

    static void Set(CalibratorMode::E calibratorMode);
};


struct TrigLev
{
    static void Load();

    static void Set(TrigSource::E ch, int16 trigLev);

    static int16 Get(TrigSource::E ch);
    static int16 GetA();
    static int16 GetB();
    // Возвращает уровень синхронизации по текущему источнику синхронизации
    static int16 Get();

    // Найти и установить уровень синхронизации по последнему считанному сигналу
    static void FindAndSet();

    static bool need_auto_find;   // Если true, то нужно произвести поиск синхронизации
};


struct RegUPR
{
    static void Load();
};


// Фильтр по входу канала
struct ChannelFiltr
{
    static void Enable(const Channel &ch, bool enable);
};
