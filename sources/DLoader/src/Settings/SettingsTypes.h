#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SDisplay
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum ColorScheme
{
    ColorScheme_WhiteLetters,       // В этом случае заголовки элементов меню пишутся белым - не очень хорошо видно снизу
    ColorScheme_BlackLetters        // В этом случае заголовки элементов меню пишутся чёрным - не очень красиво выглядит
};

// Режим отрисовки сигнала.
enum ModeDrawSignal
{
    ModeDrawSignal_Lines = 0,   // линиями
    ModeDrawSignal_Points = 1   // точками
};

// Тип сетки на экране.
enum TypeGrid
{
    TypeGrid_1,
    TypeGrid_2,
    TypeGrid_3,
    TypeGrid_4,
    TG_Size
};

// Количество накоплений.
enum NumAccumulation
{
    NumAccumulation_1,
    NumAccumulation_2,
    NumAccumulation_4,
    NumAccumulation_8,
    NumAccumulation_16,
    NumAccumulation_32,
    NumAccumulation_64,
    NumAccumulation_128,
    NumAccumulation_Infinity
};

// Количество усреднений по измерениям.
enum NumAveraging
{
    NumAveraging_1,
    NumAveraging_2,
    NumAveraging_4,
    NumAveraging_8,
    NumAveraging_16,
    NumAveraging_32,
    NumAveraging_64,
    NumAveraging_128,
    NumAveraging_256,
    NumAveraging_512
};

// Тип усреднений по измерениям
enum ModeAveraging
{
    Averaging_Accurately,   // Усреднять точно.
    Averaging_Around        // Усреднять приблизительно.
};

// Количество измерений для расчёта минимального и максимального значений.
enum NumMinMax
{
    NumMinMax_1,
    NumMinMax_2,
    NumMinMax_4,
    NumMinMax_8,
    NumMinMax_16,
    NumMinMax_32,
    NumMinMax_64,
    NumMinMax_128
};

// Количество точек для расчёта сглаживания.
enum DisplaySmoothing
{
    DisplaySmooth_Disable,
    DisplaySmooth_2points,
    DisplaySmooth_3points,
    DisplaySmooth_4points,
    DisplaySmooth_5points,
    DisplaySmooth_6points,
    DisplaySmooth_7points,
    DisplaySmooth_8points,
    DisplaySmooth_9points,
    DisplaySmooth_10points
};

// Ограничение FPS.
enum NumSignalsInSec
{
    NumSignalsInSec_25,
    NumSignalsInSec_10,
    NumSignalsInSec_5,
    NumSignalsInSec_2,
    NumSignalsInSec_1
};

// Режим отображения пропущенных сигналов при ограничении частоты кадров.
enum MissedSignals
{
    Missed_Hide,    // Не выводить на экран
    Missed_Show,    // Выводить на экран
    Missed_Average  // Устреднять и выводить на экран
};

enum ModeAccumulation
{
    ModeAccumulation_NoReset,   // В этом режиме показываются строго N последних
    ModeAccumulation_Reset      // В этом режиме набираются N последних и потом сбрасываются
};

// Режим отображения дополнительных боковых маркеров смещения по напряжению 
enum AltMarkers
{
    AM_Hide,        // Никода не выводить
    AM_Show,        // Всегда выводить
    AM_AutoHide     // Выводить и прятать через timeShowLevels
};

// Через какое время после последнего нажатия кнопки скрывать меню
enum MenuAutoHide
{
    MenuAutoHide_None    = 0,    // Никогда
    MenuAutoHide_5       = 5,    // Через 5 секунд
    MenuAutoHide_10      = 10,   // Через 10 секунд
    MenuAutoHide_15      = 15,   // Через 15 секунд
    MenuAutoHide_30      = 30,   // Через 30 секунд
    MenuAutoHide_60      = 60    // Через 60 секунд
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ChannelX
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum Channel
{
    ChanA,
    ChanB,
    ChanA_B,
    ChanMath
};

#define NumChannels 2

// Режим канала по входу
enum ModeCouple
{
    ModeCouple_DC,      // Открытый вход
    ModeCouple_AC,      // Закрытый вход
    ModeCouple_GND      // Вход заземлён
};

// Делитель
enum Multiplier
{
    Multiplier_1,
    Multiplier_10
};

// Масштаб по напряжению
enum Range
{
    Range_2mV,
    Range_5mV,
    Range_10mV,
    Range_20mV,
    Range_50mV,
    Range_100mV,
    Range_200mV,
    Range_500mV,
    Range_1V,
    Range_2V,
    Range_5V,
    Range_10V,
    Range_20V,
    RangeSize
};

// Режим запуска
enum StartMode
{
    StartMode_Auto,     // Автоматический
    StartMode_Wait,     // Ждущий 
    StartMode_Single    // Однократный
};

// Источник синхронизации
enum TrigSource
{
    TrigSource_Channel0,    // Канал 1
    TrigSource_Channel1,    // Канал 2
    TrigSource_Ext          // Внешняя
};

// Тип синхронизацц
enum TrigPolarity
{
    TrigPolarity_Front,     // По фронту
    TrigPolarity_Back       // По срезу
};

// Вход синхронизации
enum TrigInput
{
    TrigInput_Full,         // Полный сиганл
    TrigInput_AC,           // Переменный
    TrigInput_LPF,          // ФНЧ
    TrigInput_HPF           // ФВЧ
};

enum TrigModeFind
{
    TrigModeFind_Hand,          // Уровень синхронизации устанавливается вручную или автоматически - однократным нажажтием кнопки
    TrigModeFind_Auto           // Подстройки уровня синхронизации производится автоматически после каждого нового считанного сигнала
};

// Режим длительного нажатия кнопки СИНХР
enum ModeLongPressTrig
{
    ModeLongPRessTrig_Level0,
    ModeLongPressTrig_Auto
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Функция ВР/ДЕЛ
enum FunctionTime
{
    FunctionTime_Time,
    FunctionTime_ShiftInMemory
};

// Масштаб по времени
enum TBase
{
    TBase_2ns,
    TBase_5ns,
    TBase_10ns,
    TBase_20ns,
    TBase_50ns,
    TBase_100ns,
    TBase_200ns,
    TBase_500ns,
    TBase_1us, 
    TBase_2us,
    TBase_5us,
    TBase_10us,
    TBase_20us,
    TBase_50us,
    TBase_100us,
    TBase_200us,
    TBase_500us,
    TBase_1ms,
    TBase_2ms,
    TBase_5ms,
    TBase_10ms,
    TBase_20ms,
    TBase_50ms,
    TBase_100ms,
    TBase_200ms,
    TBase_500ms,
    TBase_1s,
    TBase_2s,
    TBase_5s,
    TBase_10s,
    TBaseSize
};

// Положение точки синхронизация на сигнале
enum TPos
{
    TPos_Left,      // Привязка к левому краю
    TPos_Center,    // Привязка к центру
    TPos_Right      // Привязка к правому краю
};

// Тип выборки для режима рандомизатора.
enum SampleType
{
    SampleType_Real,   // реальное время - в построении участвуют только реально считанные точки, ничего не рассчитывается
    SampleType_Equal   // эквивалентная - сигнал строится по последним точкам, полученным от рандомизатора
};

enum PeackDetMode
{
    PeackDet_Disable,
    PeackDet_Enable,
    PeackDet_Average
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cursors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Каким курсором управлять
enum CursCntrl
{
    CursCntrl_1,        // первым
    CursCntrl_2,        // вторым
    CursCntrl_1_2,      // обоими
    CursCntrl_Disable   // никаким
};

// Дискретность перемещения курсоров
enum CursMovement
{
    CursMovement_Points,    // по точкам
    CursMovement_Percents   // по процентам
};

// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
enum CursActive
{
    CursActive_U,
    CursActive_T,
    CursActive_None
};

// Режим слежения курсоров
enum CursLookMode
{
    CursLookMode_None,          // Курсоры не следят
    CursLookMode_Voltage,       // Курсоры следят за напряжением автоматически
    CursLookMode_Time,          // Курсоры следят за временем автоматически
    CursLookMode_Both           // Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Число точек сигнала, с которым идёт работа.
enum FPGA_NUM_POINTS
{
    FNP_281,
    FNP_512,
    FNP_1024
};

// Режим работы.
enum ModeWork
{
    ModeWork_Direct,        // Основной режим.
    ModeWork_Latest,        // В этом режиме можно просмотреть последние сохранённые измерения
    ModeWork_MemInt,        // В этом режиме можно сохранять во flash-памяти измерения просматривать ранее сохранённые
};

// Что показывать в режиме Внутр ЗУ - считанный или записанный сигнал
enum ModeShowIntMem
{
    ModeShowIntMem_Direct,  // Показывать данные реального времени.
    ModeShowIntMem_Saved,   // Показывать сохранённые данные
    ModeShowIntMem_Both     // Показывать данные реального времени и сохранённые данные
};

// Что делать при нажатии кнопки ПАМЯТЬ
enum ModeBtnMemory
{
    ModeBtnMemory_Menu,     // Будет открывааться соответствующая страница меню
    ModeBtnMemory_Save      // Сохранение содержимого экрана на флешку
};

// Режим наименования файлов
enum FileNamingMode
{
    FileNamingMode_Mask,        // Именовать по маске
    FileNamingMode_Manually     // Именовать вручную
};

// Как сохранять данные на флешку
enum ModeSaveSignal
{
    ModeSaveSignal_BMP,
    ModeSaveSignal_TXT
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Measures
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Сжимать ли сигналы при выводе измерений.
enum ModeViewSignals
{
    ModeViewSignals_AsIs,       // Показывать сигналы как есть
    ModeViewSignals_Compress    // Сжимать сетку с сигналами
};

enum MeasuresNumber
{
    MN_1,                       // 1 измерение слева внизу
    MN_2,                       // 2 измерения слева внизу
    MN_1_5,                     // 1 строка с 5 измерениями
    MN_2_5,                     // 2 строки по 5 измерений
    MN_3_5,                     // 3 строки по 5 измерений
    MN_6_1,                     // 6 строк по 1 измерению
    MN_6_2                      // 6 строк по 2 измерения
};

// Зона, по которой считаются измрения
enum MeasuresField
{
    MeasuresField_Screen,       // Измерения будут производиться по той части сингала, которая видна на экране.
    MeasuresField_AllMemory,    // Измерения будут производиться по всему сигналу.
    MeasuresField_Hand          // Измерения будут производиться по окну, задаваемому пользователем.
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum Function
{
    Function_Sum,
    Function_Mul
};

enum ScaleMath
{
    ScaleMath_Channel0,     // Масштаб берётся из канала 1
    ScaleMath_Channel1,     // Масштаб берётся из канала 2
    ScaleMath_Hand          // Масштаб задаётся вручную.
};

enum ScaleFFT
{
    ScaleFFT_Log,           // Это значение означает логарифмическую шкалу вывода спектра
    ScaleFFT_Linear         // Это значение означает линейную шкалу вывода спектра
};

enum SourceFFT
{
    SourceFFT_Channel0,
    SourceFFT_Channel1,
    SourceFFT_Both
};

enum WindowFFT
{
    WindowFFT_Rectangle,
    WindowFFT_Hamming,
    WindowFFT_Blackman,
    WindowFFT_Hann
};

enum FFTmaxDB
{
    FFTmaxDB_40,
    FFTmaxDB_60,
    FFTmaxDB_80
};

enum ModeDrawMath
{
    ModeDrawMath_Disable,
    ModeDrawMath_Separate,
    ModeDrawMath_Together
};

enum ModeRegSet
{
    ModeRegSet_Range,
    ModeRegSet_RShift
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Service
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Язык меню
enum Language
{
    Russian,    // Русский
    English     // Английский
};

enum CalibratorMode
{
    Calibrator_Freq,
    Calibrator_DC,
    Calibrator_GND
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Debug
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Тип балансировки АЦП каналов.
// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение для одного из АЦП канала.
enum BalanceADCtype
{
    BalanceADC_Disable,     // Балансировка выключена.
    BalanceADC_Settings,    // Используются значения балансировки, которые получены автоматически.
    BalanceADC_Hand         // Используются значения балансировки, заданные вручную.
};

// Тип растяжки АЦП
enum StretchADCtype
{
    StretchADC_Disable,
    StretchADC_Settings,
    StretchADC_Hand
};

// Тип дополнительного смещения
enum RShiftADCtype
{
    RShiftADC_Disable,
    RShiftADC_Settings,
    RShiftADC_Hand
};
