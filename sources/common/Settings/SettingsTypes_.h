// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "common/Utils/Containers/String_.h"
#include "common/Utils/Containers/Values_.h"


//#define NumChannels 2


// Тип выборки для режима рандомизатора.
struct SampleType {
    enum E
    {
        Real,   // реальное время - в построении участвуют только реально считанные точки, ничего не рассчитывается.
        Equal   // эквивалентная - сигнал строится по последним точкам, полученным от рандомизатора.
    };

    static SampleType::E Get();
};


// Число точек сигнала, с которым идёт работа.
struct EnumPointsFPGA
{
    enum E
    {
        _281,
        _512,
        _1k,
        _2k,
        _4k,
        _8k,
        _16k,
        Count
    } value;

    EnumPointsFPGA(E v = _1k) : value(v) {}

    static uint ToPoints(E);
    static E FromPoints(uint num_points);

    uint PointsInChannel();
    uint BytesInChannel();
    // Столько байт нужно для сохранения данных обоих каналов
    uint BytesForData();

    operator E() const { return value; }
};


// Источинк сигнала для отображения спектра.
struct SourceFFT {
    enum E
    {
        A_,          // Расчёт и отображение спектра первого канала.
        B_,          // Расчёт и отображение спектра второго канала.
        Both_        // Расчёт и отображение спектров обоих каналов.
    } value;
};


// Язык меню
struct Language { enum E
{
    Russian,    // Русский
    English     // Английский
};};


// Тип растяжки АЦП
struct StretchADCtype
{
    enum E
    {
        Disable,
        Settings,
        Hand
    };

    static bool IsDisabled();
    static bool IsHand();
    static bool IsSettings();
};


// Режим отрисовки сигнала.
struct ModeDrawSignal
{
    enum E
    {
        Lines,      // Сигнал рисуется линиями.
        Points,     // Сигнал рисуется точками.
        Count
    } value;

    ModeDrawSignal(E v = Lines) : value(v) {}

    bool IsLines() const { return value == Lines; };

    operator E() const { return value; }
};


// Тип сетки на экране.
struct TypeGrid
{
    enum E
    {
        _1,
        _2,
        _3,
        _4,
        Count
    } value;

    bool Is1() const { return value == _1; };
    bool Is2() const { return value == _2; };
    bool Is3() const { return value == _3; };

    operator E() const { return value; }
};


// Цветовая схема
struct ColorScheme {
    enum E
    {
        WhiteLetters,   // В этом случае заголовки элементов меню пишутся белым - не очень хорошо видно снизу.
        BlackLetters    // В этом случае заголовки элементов меню пишутся чёрным - не очень красиво выглядит.
    };

    static E Get();

    static bool IsWhiteLetters();
};


// Перечисление накоплений.
struct ENumAccumulation
{
    enum E
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
    } value;

    ENumAccumulation(E v = _1) : value(v) {}

    bool IsNone() const     { return value == _1; };
    bool IsInfinity() const { return value == Infinity; };

    operator E() const { return value; }
};


// Количество усреднений по измерениям.
struct ENumAveraging
{ 
    enum E
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
    } value;

    ENumAveraging(E v = _1) : value(v) {}

    int NumAverages();
    int ToAbs();

    operator E() const { return value; }
};


// Тип усреднений по измерениям.
struct ModeAveraging
{
    enum E
    {
        Accurately,   // Усреднять точно.
        Around        // Усреднять приблизительно.
    } value;

    ModeAveraging(E v = Accurately) : value(v) {}

    // Возвращает режим усреднения
    static ModeAveraging::E Current();

    operator E() const { return value; }
};


// Количество измерений для расчёта минимального и максимального значений.
struct ENumMinMax
{
    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128
    } value;

    ENumMinMax(E v = _1) : value(v) {}

    int ToAbs();

    operator E() const { return value; }
};


// Количество точек для расчёта скользящего фильтра.
struct Smoothing {
    
    enum E
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
    } value;

    Smoothing(E v = Disable) : value(v) {}

    // Возвращает число точек сглаживающего фильтра (режим ДИСПЛЕЙ - Сглаживание).
    static uint NumPoints();

    bool IsDisable() const { return value == Disable; }

    operator E() const { return value; }
};


// Ограничение FPS.
struct ENumSignalsInSec
{
    enum E
    {
        _25,
        _10,
        _5,
        _2,
        _1,
        Count
    } value;

    ENumSignalsInSec(E v = _25) : value(v) {}

    // Возвращает ограничение частоты кадров.
    int NumSignalsInS();

    // Возвращает промежуток времени между отрисовками
    uint DeltaTime();

    operator E() const { return value; }
};


// Режим накопления.
struct ModeAccumulation
{
    enum E
    {
        NoReset,   // В этом режиме показываются строго N последних измерений.
        Reset      // В этом режиме набираются N последних измерений и потом сбрасываются.
    } value;

    ModeAccumulation(E v = NoReset) : value(v) {}

    bool IsNoReset() const { return value == NoReset; };
    bool IsReset()   const { return value == Reset; };

    operator E() const { return value; }
};


// Режим отображения дополнительных боковых маркеров смещения по напряжению.
struct AltMarkers
{
    enum E
    {
        Hide,        // Никода не выводить.
        Show,        // Всегда выводить.
        AutoHide     // Выводить и прятать через timeShowLevels.
    } value;

    AltMarkers(E v = Hide) : value(v) {}

    bool IsHide() const { return value == Hide; };
};


// Через какое время после последнего нажатия кнопки скрывать меню.
struct MenuAutoHide
{
    enum E
    {
        None,   // Никогда.
        _5,     // Через 5 секунд.
        _10,    // Через 10 секунд.
        _15,    // Через 15 секунд.
        _30,    // Через 30 секунд.
        _60     // Через 60 секунд.
    } value;

    MenuAutoHide(E v = None) : value(v) {}

    // Возвращает время, через которое меню автоматически скрывается, если не было больше нажатий.
    int Time();
};


// Тип привязки к смещению по вертикали
struct LinkingRShift
{
    enum E
    {
        Voltage,      // Смещение привязано к напряжению
        Position      // Смещение привязаоно к позиции на экране
    } value;

    bool IsVoltage() const { return value == Voltage; };
};


// Количество и расположение на экране результатов измерений.
struct MeasuresNumber
{
    enum E
    {
        _1,             // 1 измерение слева внизу.
        _2,             // 2 измерения слева внизу.
        _1_5,           // 1 строка с 5 измерениями.
        _2_5,           // 2 строки по 5 измерений.
        _3_5,           // 3 строки по 5 измерений.
        _6_1,           // 6 строк по 1 измерению.
        _6_2            // 6 строк по 2 измерения.
    } value;

    bool Is1()   const { return value == _1; };
    bool Is6_1() const { return value == _6_1; };
    bool Is6_2() const { return value == _6_2; };
    bool Is1_5() const { return value == _1_5; };
    bool Is2_5() const { return value == _2_5; };
    bool Is3_5() const { return value == _3_5; };

    operator int() const { return (int)value; }
};


// Зона, по которой считаются измрения
struct MeasuresField
{
    enum E
    {
        Screen,         // Измерения будут производиться по той части сингала, которая видна на экране.
        AllMemory,      // Измерения будут производиться по всему сигналу.
        Hand            // Измерения будут производиться по окну, задаваемому пользователем.
    } value;

    bool IsHand() const { return value == Hand; };
};


// Режим работы.
struct ModeWork
{
    enum E
    {
        Direct,        // Основной режим.
        Latest,        // В этом режиме можно просмотреть последние сохранённые измерения.
        MemInt,        // В этом режиме можно сохранять во flash-памяти измерения просматривать ранее сохранённые.
    } value;

    ModeWork(E v = Direct) : value(v) {}

    bool IsDirect() const { return value == Direct; };
    bool IsLatest() const { return value == Latest; };
    bool IsMemInt() const { return value == MemInt; };

    operator E() const { return value; }
};


// Режим наименования файлов.
struct FileNamingMode
{
    enum E
    {
        Mask,        // Именовать по маске.
        Manually     // Именовать вручную.
    } value;

    FileNamingMode(E v = Mask) : value(v) {}

    bool IsMask() const { return value == Mask; };
    bool IsHand() const { return value == Manually; };
};


// Что показывать в режиме Внутр ЗУ - считанный или записанный сигнал.
struct ModeShowIntMem
{
    enum E
    {
        Direct,  // Показывать данные реального времени.
        Saved,   // Показывать сохранённые данные.
        Both     // Показывать данные реального времени и сохранённые данные.
    } value;

    ModeShowIntMem(E v = Direct) : value(v) {}

    bool IsDirect() const { return value == Direct; }
    bool IsSaved() const { return value == Saved; }
    bool IsBoth() const { return value == Both; }
};


// Что делать при нажатии кнопки ПАМЯТЬ.
struct ModeBtnMemory
{
    enum E
    {
        Menu,     // Будет открывааться соответствующая страница меню.
        Save      // Сохранение содержимого экрана на флешку.
    } value;

    ModeBtnMemory(E v = Menu) : value(v) {}

    bool IsSave() const { return value == Save; };
};


// Как сохранять данные на флешку.
struct ModeSaveSignal
{
    enum E
    {
        BMP,     // Сохранять данные на флешку в формате .bmp.
        TXT      // Сохранять данные на флешку в текствовом виде.
    } value;

    ModeSaveSignal(E v = BMP) : value(v) {}

    bool IsBMP() const { return value == BMP; }
};


// Функция ручки ВРЕМЯ/ДЕЛ.
struct FunctionTime {
    enum E
    {
        Time,              // Ручка управляет смещением по времени.
        ShiftInMemory      // Ручка управляет отображаемым на экране участком памяти.
    };

    static E Get();
    static bool IsShiftInMemory();
};


// Режим работы пикового детектора.
struct PeackDetMode
{
    enum E
    {
        Disable,
        Enable,
        Average,
        Count
    } value;

    PeackDetMode(E v = Disable) : value(v) {}

    static void Set(PeackDetMode::E mode);
    static PeackDetMode::E Get();
    static bool IsEnabled();

    operator E() const { return value; }
};


// Привязка синхронизации к памяти.
struct TPos
{
    enum E
    {
        Left,      // Синхронизация привязана к началу памяти.
        Center,    // Синхронизация привязана к центру памяти.
        Right,     // Синхронизация привязана к концу памяти.
        Count
    } value;

    TPos(E v = Center) : value(v) {}

    static bool IsLeft();
    static bool IsCenter();
    static bool IsRight();

    // Узнать привязку отсительно уровня синхронизации в точках
    static int InPoints(EnumPointsFPGA::E enum_points, E t_pos);

    static void Draw();

    static void Draw(int x, int y);

    operator E() const { return value; }
};


struct Channel
{
    enum E
    {
        A,
        B,
        A_B,
        Math,
        Count = 2
    } value;

    Channel(E v = A) : value(v) {}
    void Enable() const;
    void Disable() const;
    bool IsEnabled() const;
    bool IsA() const     { return (value == A); }
    bool IsB() const     { return (value == B); }
    bool IsAB() const    { return (value == A_B); }
    bool IsMath() const  { return (value == Math); }
    operator int() const { return (int)value; }
    Color GetColor() const;
    bool IsInversed() const;
    int ToNumber() const;

    // Данные этого канала нужно отрисовывать первыми
    static Channel &FirstDrawing();

    // Данные этого канала нужно отрисовывать последними
    static Channel &SecondDrawing();
};

extern Channel ChA;
extern Channel ChB;


// Делитель.
struct Divider
{
    enum E
    {
        _1,
        _10
    } value;

    Divider(E v = _1) : value(v) {}

    operator E() const { return value; }

    static int ToAbs(Divider::E divider);
    static int ToAbs(const Channel &ch);
};

// Режим канала по входу.
struct ModeCouple
{
    enum E
    {
        DC,      // Открытый вход.
        AC,      // Закрытый вход.
        GND      // Вход заземлён.
    } value;

    ModeCouple(E v = DC) : value(v) {}

    operator E() const { return value; }

    static void Set(const Channel &ch, ModeCouple::E mode);
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
    } value;

    Range(E v = _2mV) : value(v) {}

    operator E() const { return value; }

    static void Load(const Channel &ch);

    static void Set(const Channel &ch, Range::E range);

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

    static int16 GetMath();

    static void Load(const Channel &ch);

    static int ToRel(float rshift_abs, Range::E range);

    static Voltage ToAbs(int16 rshift, Range::E range);

    static Voltage ToAbs(int16 rshift, Range::E range, Divider::E multiplier);

    static void Draw();

    static void DrawCursors();

    static bool show_level[Channel::Count];    // Нужно ли рисовать горизонтальную линию уровня смещения первого канала
    static bool draw_markers;

    static const int16 MIN = 20;    // Минимальное значение смещения канала по напряжению, засылаемое в аналоговую часть
                                    // Соответствует смещению 10 клеток вниз от центральной линии
    static const int16 ZERO = 500;  // Соответствует расположению марера по центру экрана.
    static const int16 MAX = 980;   // Соответствует смещению 10 клеток вверх от центральной лиини.

    static int16 STEP();            // На столько единиц нужно изменить значение смещения, чтобы маркер смещения по
                                    // напряжению передвинулся на одну точку

private:

    static void Draw(const Channel &ch);

    static void OnChanged(const Channel &ch);

    static void FuncOnTimerDisableShowA();
    static void FuncOnTimerDisableShowB();

    // Отключить вспомогательную линию маркера смещения по напряжению
    static void DisableShowLevel(const Channel &ch);

    static const float abs_step[Range::Count];
};


// Режим запуска.
struct StartMode {
    enum E
    {
        Auto,             // Автоматический.
        Wait,             // Ждущий.
        Single            // Однократный.
    } value;

    StartMode(E v = Auto) : value(v) {}

    bool IsAuto() const { return value == Auto; };
    bool IsSingle() const { return value == Single; }

    operator E() const { return value; }
};

// Источник синхронизации.
struct TrigSource
{
    enum E
    {
        A,      // Канал 1.
        B,      // Канал 2.
        Ext     // Внешняя.
    } value;

    static void Set(E source);

    static E Get();
    static bool IsA();
    static bool IsB();
    static bool IsExt();
};


// Полярность синхронизацц
struct TrigPolarity
{
    enum E
    {
        Front,  // Синхронизация по фронту.
        Back    // Синхронизация по срезу.
    } value;

    TrigPolarity(E v = Front) : value(v) {};

    static void Set(E polarity);

    bool IsFront() const { return value == Front; };

    // Переключение полярности - принудетиельный запуск сбора информации
    static void Switch();

    static void Load();

    operator E() const { return value; }

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
    } value;

    static void Set(E input);

    static E Get();

    static bool IsAC();
};


// Режим длительного нажатия кнопки СИНХР.
struct ModeLongPressTrig {
    enum E
    {
        Level0,  // Сброс уровня синхронизации в 0.
        Auto     // Автоматический поиск синхронизации - уровень устанавливается посередине между максимумом и минимумом
    } value;

    static E Get();

    static bool IsLevel0();
};


// Режим установки синхронизации.
struct TrigModeFind {
    enum E
    {
        Hand,   // Уровень синхронизации устанавливается вручную.
        Auto    // Подстройки уровня синхронизации производится автоматически после каждого нового считанного сигнала
    } value;

    static E Get();

    static bool IsHand();
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
    } value;

    TBase(E v = TBase::_1ns) : value(v) {}

    static void Set(TBase::E tbase);

    static void Load();

    static void Decrease();

    static void Increase();

    // Возвращает true, если установлена рандомизаторная развёртка
    static bool IsRandomize();

    // Возвращает true, если установлена поточечная развёртка
    static bool IsP2P();

    // Возвращает true, если разрешён режим пикового детектора
    static bool IsPeakDetModeAllowed();

    static pchar Name(TBase::E tbase);

    static pchar ToString(TBase::E tbase);
    static pchar ToStringEN(TBase::E tbase);

    static E MIN_P2P;       // С этой развёртки начинается поточечный вывод
    static E MIN_PEAK_DET;  // С этой развёртки можно включать пиковый детектор
    static E MAX_RANOMIZE;  // Максимальная развёртка, на которой работает рандомизатор

    // Возвращает шаг между точками для рандомизатора при установленной развёртке
    static int StepRand();

    operator E() const { return value; }
};


struct TShift
{
    static void Set(int tshift);

    static void Draw();

    // Установить добавочное смещение по времени для режима рандомизатора. В каждой развёртке это смещение должно быть
    // разное.
    static void SetDelta(int16 shift);

    // Возвращает установленное смещение по времени в текстовом виде, пригодном для вывода на экран
    static String ToString(int tshift_rel);

    // Минимальное смещение по времени, которое может быть записано в аппаратную часть.
    static int16 Min();

    // Смещение по времени, соответствующее позиции TPos.
    static int16 Zero();

    // Смещение по времени, используемое для засылки в пред/после- запуски
    static int ForLaunchFPGA();

    // tshift может принимать нецелые значения в результате расчёта
    static Time ToAbs(float shift, TBase::E base);
    static Time ToAbs(int shift, TBase::E base);

    static float ToRel(float tshift_abs, TBase::E tbase);

    static const int NULL_VALUE = 1000000;

    static const int16 MAX = 16000;

    static const float abs_step[TBase::Count];

    static int add_rand;                // Дополнительное смещение в режиме рандомизатора
};


// Каким курсором управлять.
struct CursCntrl
{
    enum E
    {
        _1,            // первым.
        _2,            // вторым.
        _1_2,          // обоими.
        Disable        // никаким.
    } value;

    bool IsDisable() const { return value == Disable; }
    bool Is1()       const { return value == _1; }
    bool Is2()       const { return value == _2; }
    bool Is1_2()     const { return value == _1_2; }
};


// Дискретность перемещения курсоров.
struct CursMovement
{
    enum E
    {
        Points,    // по точкам.
        Percents   // по процентам.
    } value;

    bool IsPercents() const { return value == Percents; };
};


// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
struct CursActive
{
    enum E
    {
        U,
        T,
        None
    } value;

    CursActive(E v = U) : value(v) {}

    bool IsT() const { return value == T; }
    bool IsU() const { return value == U; }
};


// Режим слежения курсоров.
struct CursLookMode
{
    enum E
    {
        None,      // Курсоры не следят.
        Voltage,   // Курсоры следят за напряжением автоматически.
        Time,      // Курсоры следят за временем автоматически.
        Both       // Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
    } value;

    bool IsVoltage() const { return value == Voltage; }
    bool IsTime() const    { return value == Time; }
    bool IsBoth() const    { return value == Both; }
};


// Сжимать ли сигналы при выводе измерений.
struct ModeViewSignals
{
    enum E
    {
        AsIs,       // Показывать сигналы как есть
        Compress    // Сжимать сетку с сигналами
    } value;

    bool IsCompress() const { return value == Compress; };
};


struct ScaleMath { enum E
{
    Channel0,     // Масштаб берётся из канала 1
    Channel1,     // Масштаб берётся из канала 2
    Hand          // Масштаб задаётся вручную.
};};


// Тип балансировки АЦП каналов.
// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение
// для одного из АЦП канала.
struct BalanceADCtype
{
    enum E
    {
        Disable,     // Балансировка выключена.
        Settings,    // Используются значения балансировки, которые получены автоматически.
        Hand         // Используются значения балансировки, заданные вручную.
    };

    static E Get();
};


// Тип дополнительного смещения
struct RShiftADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};


// Масштаб шкалы для отображения спектра.
struct ScaleFFT
{
    enum E
    {
        Log,        // Это значение означает логарифмическую шкалу вывода спектра.
        Linear      // Это значение означает линейную шкалу вывода спектра.
    } value;

    bool IsLog()    const { return value == Log; };
    bool IsLinear() const { return value == Linear; };
};


// Вид окна для умножения сигнала перед расчётом спектра.
struct WindowFFT
{
    enum E
    {
        Rectangle,  // Перед расчётом спектра сигнал не умножается на окно.
        Hamming,    // Перед расчётом спектра сигнал умножается на окно Хэмминга.
        Blackman,   // Перед расчётом спектра сигнал умножается на окно Блэкмана.
        Hann        // Перед расчётом спектра сигнал умножается на окно Ханна.
    } value;

    bool IsHamming()  const { return value == Hamming; };
    bool IsBlackman() const { return value == Blackman; };
    bool IsHann()     const { return value == Hann; };
};


// Минимальное значение на вертикальной координате графика спектра.
struct MaxDbFFT
{
    enum E
    {
        _40,        // Вертикальная координата до -40Дб
        _60,        // Вертикальная координата до -60Дб
        _80         // Вертикальная координата до -80Дб
    } value;

    float GetReal();
    operator E() const { return value; }
};


// Какую функцию рассчитывать.
struct Function
{
    enum E
    {
        Sum,        // Сумма двух каналов.
        Mul         // Произведение двух каналов.
    } value;

    bool IsSum() const { return value == Sum; };
    bool IsMul() const { return value == Mul; };
    operator E() const { return value; }
};


// Где отрисовывать математический сигнал.
struct ModeDrawMath
{
    enum E
    {
        Disable,    // Отображение математического сигнала выключено.
        Separate,   // Математический сигнал отображается на отдельном экране.
        Together    // Математический сигнал отображается поверх текущего сигнала.
    };

    static E Get();

    static bool IsDisabled();
    static bool IsSeparate();
};


// Чем управляет ручка УСТАНОВКА, когда включена математическая функция.
struct ModeRegSet
{
    enum E
    {
        Range,      // Ручка УСТАНОВКА управляет масштабом по напряжению.
        RShift      // Ручка УСТАНОВКА управляет смещением по напряжению.
    };

    static E Get();

    static bool IsRShift();
    static bool IsRange();
};


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

    static E Get();
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

    static void DrawCursor();

    static void WriteValue();

    static bool need_auto_find;   // Если true, то нужно произвести поиск синхронизации

    static const int16 MIN = RShift::MIN;
    static const int16 MAX = RShift::MAX;
    static const int16 ZERO = RShift::ZERO;     // Нулевое значение уровня синхронизации, засылаемое в прибор.
                          // Маркер синхронизации при этом находитися на одном уровне с маркером смещения по напряжению.

    static bool exist_impulse;          // Если true, нужно отображать значок наличия синхроимпульса

    static bool show_level;             // Нужно ли рисовать горизонтальную линию уровня смещения уровня синхронизации
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


// Структура для управления пред/после- запусками
struct LaunchFPGA
{
    // Загружает в аппаратную часть пред- и после- запуски
    static void Load();

    static uint16 Pred() { return (uint16)pred; }

    // Возвращают значения, готовые для записи в ПЛИС
    static uint16 PredForWrite();
    static uint16 PostForWrite();

    // На сколько менять адрес первого чтения
    static int DeltaReadAddress();

    // Дополнительное смещение для первой считываемой точки (нужно для того, чтобы учесть, что в режиме рандомизатора
    // изменение tShift вызывает перемещение по экрану не на 1 пиксель)
    static int AdditionalOffsetIndexFirst();

private:

    static void Calculate();

    static void CalculateReal();

    static void CalculateRandomize();

    static int pred;
    static int post;

    static const int8 d_pred[TBase::Count];   // Дополнительное смещение для предзапуска
    static const int8 d_post[TBase::Count];   // Дополнительное смещение для послезапуска
    static const int8 d_read[TBase::Count];   // Дополнительное смещение для чтения адреса
};


struct CalibrationMode
{
    enum E
    {
        x1,
        x10,
        Disable
    };
};
