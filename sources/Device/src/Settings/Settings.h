#pragma once
#include "Menu/Items/MenuItems.h"
#include "Utils/Measures.h"


#define RSHIFT_ADD(ch, range, mode) set.chan[ch].rshift_add[range][mode]

#define MAC_ADDR0       (set.eth.mac0)
#define MAC_ADDR1       (set.eth.mac1)
#define MAC_ADDR2       (set.eth.mac2)
#define MAC_ADDR3       (set.eth.mac3)
#define MAC_ADDR4       (set.eth.mac4)
#define MAC_ADDR5       (set.eth.mac5)
//
#define IP_ADDR0        (set.eth.ip0)
#define IP_ADDR1        (set.eth.ip1)
#define IP_ADDR2        (set.eth.ip2)
#define IP_ADDR3        (set.eth.ip3)
//
#define PORT_ETH        (set.eth.port)
//
#define NETMASK_ADDR0   (set.eth.mask0)
#define NETMASK_ADDR1   (set.eth.mask1)
#define NETMASK_ADDR2   (set.eth.mask2)
#define NETMASK_ADDR3   (set.eth.mask3)
//
#define GW_ADDR0        (set.eth.gw0)
#define GW_ADDR1        (set.eth.gw1)
#define GW_ADDR2        (set.eth.gw2)
#define GW_ADDR3        (set.eth.gw3)


#define LANG            (set.common.lang)       // SettingsCommon.lang
#define LANG_RU         (LANG == Language::Russian)
#define LANG_EN         (LANG == Language::English)

#define BALANCE_ADC(ch)                     (set.debug.balanceADC[ch])
#define BALANCE_ADC_A                       (BALANCE_ADC(ChA))
#define BALANCE_ADC_B                       (BALANCE_ADC(ChB))

#define DEBUG_STRETCH_ADC_TYPE              (set.debug.stretchADCtype)
#define DEBUG_STRETCH_ADC_TYPE_IS_DISABLED  (DEBUG_STRETCH_ADC_TYPE == StretchADCtype::Disable)
#define DEBUG_STRETCH_ADC_TYPE_IS_HAND      (DEBUG_STRETCH_ADC_TYPE == StretchADCtype::Hand)
#define DEBUG_STRETCH_ADC_TYPE_IS_SETTINGS  (DEBUG_STRETCH_ADC_TYPE == StretchADCtype::Settings)

#define DEBUG_STRETCH_ADC(ch)               (set.debug.stretchADC[ch])
#define DEBUG_STRETCH_ADC_A                 (DEBUG_STRETCH_ADC(ChA))
#define DEBUG_STRETCH_ADC_B                 (DEBUG_STRETCH_ADC(ChB))

#define NUM_AVE_FOR_RAND                    (::set.debug.numAveForRand)


#define IP_ADDRESS          (set.service.IPaddress)             // SettingsService.IPaddress


#define IS_SHOW_REGISTERS_ALL   (set.debug.showRegisters.all)
#define IS_SHOW_REG_RSHIFT_A    (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.rShiftA)
#define IS_SHOW_REG_RSHIFT_B    (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.rShiftB)
#define IS_SHOW_REG_TRIGLEV     (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.trigLev)
#define IS_SHOW_REG_RANGE_A     (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.range[Channel::A])
#define IS_SHOW_REG_RANGE_B     (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.range[Channel::B])
#define IS_SHOW_REG_TRIGPARAM   (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.trigParam)
#define IS_SHOW_REG_PARAM_A     (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.chanParam[Channel::A])
#define IS_SHOW_REG_PARAM_B     (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.chanParam[Channel::B])
#define IS_SHOW_REG_TSHIFT      (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.tShift)
#define IS_SHOW_REG_TBASE       (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.tBase)


#define PRETRIGGERED            (set.debug.pretriggered)


#define FILE_NAME_MASK          (set.memory.fileNameMask)


 // Настройки каналов
struct SettingsChannel
{
    float         stretch_ADC;                 // Поправочный коэффициент.
    int16         rshift;
    int16         rshift_add[Range::Count][2]; // Добавочное смещение для открытого (0) и закрытого (1) входов.
    ModeCouple::E modeCouple;                  // Режим по входу.
    Divider::E    divider;                     // Множитель.
    Range::E      range;                       // Масштаб по напряжению.
    bool          enable;                      // Включён ли канал.
    bool          inverse;                     // Инвертирован ли канал.
    bool          filtr;                       // Фильтр
    int8          balanceShiftADC;             // Добавочное смещение для балансировки АЦП

    static bool FiltrIsEnabled(const Channel &ch);
    static float StretchADC(const Channel &ch);
    static int8 BalanceShiftADC(const Channel &ch);
};


struct SettingsEthernet
{
    uint8 mac0;
    uint8 mac1;
    uint8 mac2;
    uint8 mac3;
    uint8 mac4;
    uint8 mac5;

    uint8 ip0;
    uint8 ip1;
    uint8 ip2;
    uint8 ip3;

    uint16 port;

    uint8 mask0;
    uint8 mask1;
    uint8 mask2;
    uint8 mask3;

    uint8 gw0;
    uint8 gw1;
    uint8 gw2;
    uint8 gw3;

    bool enabled;
};

                                   
struct SettingsCommon
{
    int         countEnables;               // Количество включений. Увеличивается при каждом включении.
    int         countErasedFlashData;       // Сколько раз стирался первый сектор с ресурсами.
    int         countErasedFlashSettings;   // Сколько раз стирался сектор с настройкаи.
    int         workingTimeInSecs;          // Время работы в секундах.
    Language::E lang;                       // Язык меню.
};



struct OutputRegisters
{
    bool    all;            // Показывать значения всех регистров.
    bool    flag;           // Выводить ли флаг готовности.
    bool    rShiftA;
    bool    rShiftB;
    bool    trigLev;
    bool    range[2];
    bool    chanParam[2];
    bool    trigParam;
    bool    tShift;
    bool    tBase;
};


struct StructMemoryLast
{
    bool isActiveModeSelect;
};


// Настройки МЕНЮ->ПАМЯТЬ
struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    ENUM_POINTS_FPGA::E enum_points_fpga;                       // Число точек.
    ModeWork::E         modeWork;                               // Режим работы.
    FileNamingMode::E   fileNamingMode;                         // Режим именования файлов.
    char                fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; // Здесь маска для автоматического именования файлов\n
        // Правила именования.\n
        // %y('\x42') - год, %m('\x43') - месяц, %d('\x44') - день, %H('\x45') - часы, %M('\x46') - минуты, %S('\x47') - секунды\n
        // %Nn('\x48''n') - порядковый номер, котрый занимает не менее n знакомест, например, 7 в %3N будет преобразовано в 007\n
        // Примеры\n
        // name_%4N_%y_%m_%d_%H_%M_%S будет генерировать файлы вида name_0043_2014_04_25_14_45_32\n
        // При этом обратите внимание, что если спецификатор %4N стоИт после временнЫх параметров, то, скорее всего,
        //этот параметр будет всегда равен 0001, т.к. для определения номера просматриваются только символы ДО него.
    char                file_name[MAX_SYMBOLS_IN_FILE_NAME];    // Имя файла для режима ручного задания
    int8                index_cur_symbol_name_mask;             // Индекс текущего символа в режиме задания маски или
                                                                // выбора имени.
    StructMemoryLast    strMemoryLast;
    ModeShowIntMem::E   modeShowIntMem;                         // Какие сигналы показывать в режиме внутреннего ЗУ
    bool                flash_auto_connect;                     // Если true, при подлючении флеш автоматически
                                                                // выводится NC (Нортон Коммандер)
    ModeBtnMemory::E    modeBtnMemory;
    ModeSaveSignal::E   modeSaveSignal;                         // В каком виде сохранять сигнал.

    static int GetNumPoints(bool forCalculate);
    static ENUM_POINTS_FPGA::E IntNumPoints2FPGA_NUM_POINTS(int numPoints);
};


// Настройки изображения
struct SettingsDisplay
{
    int16               time_show_levels;       // Время, в течение которого нужно показывать уровни смещения.
    int16               shift_in_memory;        // Показывает смещение левого края сетки относительно нулевого байта
                                                // памяти. Нужно для правильного отображения сигнала в окне.
    int16               time_messages;          // Время в секундах, на которое сообщение остаётся на экране.
    int16               brightness;             // Яркость дисплея.
    int16               brightness_grid;        // Яркость сетки от 0 до 100.
    uint                colors[16];             // Цвета.
    ModeDrawSignal::E   mode_draw_signal;       // Режим отрисовки сигнала.
    TypeGrid::E         typeGrid;               // Тип сетки.
    ENumAccumulation::E enum_accum;             // Перечисление накоплений сигнала на экране.
    ENumAveraging::E    enum_ave;               // Перечисление усреднений сигнала.
    ModeAveraging::E    mode_ave;               // Тип усреднений по измерениям.
    ENumMinMax::E       enum_min_max;           // Число измерений для определения минимумов и максимумов.
    Smoothing::E        smoothing;              // Число точек для скользящего фильтра.
    ENumSignalsInSec::E enum_fps;               // Перечисление считываний сигнала в секунду.
    Channel::E          last_affected_channel;  // Здесь хранится номер последнего канала, которым управляли ручками.
                                                // Нужно для того, чтобы знать, какой сигнал рисовать наверху.
    ModeAccumulation::E mode_acum;              // Задаёт режим накопления сигналов.
    AltMarkers::E       alt_markers;            // Режим отображения дополнительных боковых маркеров смещений.
    MenuAutoHide::E     menu_auto_hide;         // Через сколько времени после последнего нажатия клавиши прятать меню.
    bool                showFullMemoryWindow;   // Показывать ли окно памяти вверху экрана. \todo Не используется.
    bool                show_string_navigation; // Показывать ли строку текущего состояния меню..
    LinkingRShift::E    linking_rshift;         // Тип привязки смещения по вертикали.

    // Возвращает адрес первой и последней точки на экране.
    static void PointsOnDisplay(int *firstPoint, int *lastPoint);

    // Если экран разделён на две части и основной сигнал выводится сверху - например, в режиме вывода спектра
    static bool IsSeparate();
};


// Настройки курсорных измерений.
struct SettingsCursors
{
    CursCntrl       cntrlU[NumChannels];    // Активные курсоры напряжения.
    CursCntrl       cntrlT[NumChannels];    // Активные курсоры напряжения.
    Channel         source;                 // Источник - к какому каналу относятся курсоры.
    float           posU[NumChannels][2];   // Текущие позиции курсоров напряжения обоих каналов.
    float           posT[NumChannels][2];   // Текущие позиции курсоров времени обоих каналов.
    float           dU_100percents[2];      // Расстояние между курсорами напряжения для 100%, для обоих каналов.
    float           dT_100percents[2];      // Расстояние между курсорами времени для 100%, для обоих каналов.
    CursMovement    movement;               // Как перемещаться курсорам - по точкам или по процентам.
    CursActive      active;                 // Какие курсоры сейчас активны.
    CursLookMode    look_mode[2];           // Режимы слежения за курсорами для двух пар курсоров.
    bool            show_freq;              // Установленное в true значение, что нужно показывать на экране значение
                                            // 1/dT между курсорами.
    bool            show;                   // Показывать ли курсоры.
};


struct Cursors
{
    // Получить позицию курсора напряжения.
    static float GetPosU(const Channel &ch, int num);

    // Возвращает true,если нужно рисовать курсоры.
    static bool NecessaryDraw();

    // Получить строку курсора напряжения.
    static String GetVoltage(const Channel &source, int numCur);

    // Получить строку курсора времени.
    static String GetTime(const Channel &source, int numCur);

    // Получить строку процентов курсоров напряжения.
    static String GetPercentsU(const Channel &source);

    // Получить строку процентов курсоров времени.
    static String GetPercentsT(const Channel &source);

    // Нарисовать курсоры курсорных измерений.
    static void Draw();

private:

    // Нарисовать вертикальный курсор курсорных измерений.
    // x         - числовое значение курсора
    // y_tearing - координата места, в котором необходимо сделать разрыв для квадрата пересечения
    static void DrawVertical(int x, int yTearing);

    // Нарисовать горизонтальный курсор курсорных измерений
    // y         - числовое значение курсора
    // x_tearing - координата места, в котором необходимо сделать разрыв для квадрата пересечения
    static void DrawHorizontal(int y, int x_tearing);
};


// Настройки меню ИЗМЕРЕНИЯ
struct SettingsMeasures
{
    MeasuresNumber  number;            // Сколько измерений выводить.
    Channel         source;            // Для каких каналов выводить измерения.
    ModeViewSignals mode_view_signals; // Сжимать ли сигналы при выводе измерений.
    Measure         measures[15];      // Выбранные для индикации измерения.
    bool            show;              // Показывать ли измерения.
    MeasuresField   field;             // Задаёт область, из которой берутся значения для расчёта измерений.
    int16           posU[2];           // Позиции курсоров, которые задают область, из которой берутся значения для
                                       // расчёта измерений при field == MeasuresField::Hand.
    int16           posT[2];           // Позиции курсоров, которые задают область, из которой берутся значения для
                                       // расчёта измерений при field == MeasuresField::Hand.
    CursCntrl       cntrlU;            // Активные курсоры напряжения.
    CursCntrl       cntrlT;            // Активные курсоры времени.
    CursActive      curs_active;       // Какие курсоры активны - по времени или напряжению.
    Measure         marked;            // Измерение, на которое нужно выводить маркеры.

    static int16 PosCursorU(int num);
    static int16 PosCursorT(int num);
};


struct SettingsMath
{
    ScaleFFT::E     scaleFFT;       // Масштаб шкалы для отображения спектра.
    SourceFFT::E    sourceFFT;      // Источник сигнала для расчёта и отображения спектра.
    WindowFFT::E    windowFFT;      // Задаёт вид окна, на которое умножается сигнал перед расчётом спектра.
    FFTmaxDB::E     fftMaxDB;       // Минимальное значение на вертикальной координате графика спектра.
    Function::E     func;           // Включённая математическая функция.
    uint8           cursorFFT;      // Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
    uint8           posCur[2];      // Позиция курсора спектра. Изменятеся 0...256.
    int8            koeff1add;      // Коэффициент при первом слагаемом для сложения.
    int8            koeff2add;
    int8            koeff1mul;
    int8            koeff2mul;
    bool            enableFFT;      // \c true, если включён спектр.
    ModeDrawMath::E modeDraw;       // Раздельный или общий дисплей в режиме математической функции.
    ModeRegSet::E   modeRegSet;     // Функция ручки УСТАНОВКА - масштаб по времени или смещение по вертикали.
    Range::E        range;          // Масштаб по напряжению математического сигнала.
    Divider::E      multiplier;     // Множитель масштаба математического сигнала.
    int16           rShift;         // Смещение по вертикальной оси математического сигнала.

    static bool IsMathEnabled();
    static uint8 CursorFFT();
    static uint8 PositionCursorFFT(int num);
    static bool IsEnbaledFFT();
};



// Настройки меню СЕРВИС.
struct SettingsService
{
    bool              sound_enabled;       // Включены ли звуки.
    CalibratorMode::E calibrator;          // Режим работы калибратора.
    int8              IPaddress;           // IP-адрес (временно).
    ColorScheme::E    colorScheme;         // Цветовая схеама.
};


// Настройки синхронизации
struct SettingsTrig
{
    StartMode::E         start_mode;         // Режим запуска.
    TrigSource::E        source;             // Источник.
    TrigPolarity::E      polarity;           // Тип синхронизации.
    TrigInput::E         input;              // Вход синхронизации.
    int16                levelRel[3];        // Уровень синхронизации для трёх источников.
    ModeLongPressTrig::E modeLongPressTrig;  // Режим работы длительного нажатия кнопки СИНХР.
    TrigModeFind::E      modeFind;           // Поиск синхронизации - вручную или автоматически.
};


// Настройки оси X.
struct SettingsTime
{
    TBase::E            tBase;          // Масштаб по времени.
    int16               tShiftRel;      // Смещение по времени.
    FunctionTime::E     function_time;
    TPos::E             tPos;           // Привязка синхронизации к памяти.
    SampleType::E       sampleType;     // Тип выборки для режима рандомизатора.
    PeackDetMode::E     peakDet;        // Режим работы пикового детектора
    bool                selfRecorder;   // Включен ли режим самописца.
    ENUM_POINTS_FPGA::E oldNumPoints;   // \brief Когда переключаемся в режим пикового детектора, устанавливаем
                                        // количество точек в 1024, а сюда  записываем то, что было, чтобы потом
                                        // восстановить.
};


// Отладочные настройки.
struct SettingsDebug
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
    int16             numAveForRand;              // По скольким измерениям усреднять сигнал в режиме рандомизатора.
    bool              viewAlteraWrittingData;     // Показывать ли данные, идущие в альтеру.
    bool              viewAllAlteraWrittingData;  // \brief Показывать ли все данные, идущие в альтеру (если false, то
                                                  // постоянно идущие команды вроде 
                                                  // START, STOP не показываются).
    int16             altShift;                   // Добавочное смещение для устранения эффекта горизонтальной помехи
                                                  // синхронизации.
    int16             pretriggered;               // Регулируемая величина предзапуска для исследования рандомизатора.
    OutputRegisters   showRegisters;

    static int  GetSizeFontForConsole();        // Возвращает размер шрифта, которым нужно выводить сообщения в консоли.
};

struct SettingsMenu
{
    bool        pageDebugActive;                  // Активна ли кнопка отладки в меню.
    unsigned    isShown : 1;                      // Меню показано.
};


// Струкура хранит все настройки прибора.
struct Settings
{
    uint                size;
    SettingsDisplay     display;            // настройки изображения          (меню ДИСПЛЕЙ).
    SettingsChannel     chan[NumChannels];  // настройки каналов              (меню КАНАЛ 1 и КАНАЛ 2).
    SettingsTrig        trig;               // настройки синхронизации        (меню СИНХР).
    SettingsTime        time;               // временнЫе настройки            (меню РАЗВЁРТКА).
    SettingsCursors     cursors;            // настройки курсорных измерений  (меню КУРСОРЫ).
    SettingsMemory      memory;             // настройки режимов памяти       (меню ПАМЯТЬ).
    SettingsMeasures    measures;           // настройки измерений            (меню ИЗМЕРЕНИЯ).
    SettingsMath        math;               // настройки режима математических измерений.
    SettingsService     service;            // дополнительные настройки       (меню СЕРВИС).
    SettingsEthernet    eth;                // настройки для соединения по локальной сети.
    SettingsCommon      common;             // системные настройки.
    SettingsMenu        menu;               // состояние меню.
    SettingsDebug       debug;              // настройки режима отладки       (меню ОТЛАДКА).
    uint                crc32;              // контрольная сумма. Используется для проверки корректности сохранённых
                                            // настроек

    static void Load(bool _default);  // \brief Загрузить настройки. Если _default == true, загружаются настройки по
                                      // умолчанию, иначе пытается 
                                      // загрузить настройки из ПЗУ, а в случае неудачи - тоже настройки по умолчанию.
    static void Save();               // Сохранить настройки во флеш-память.
    static bool DebugModeEnable();    // Возвращает true, если включён режим отладки.
    static bool loaded;               // Эта переменная нужна для того, чтобы исключить ложную запись пустых настроек
                                      // из-за неправильного включения прибора (при исключённом из схемы программном
                                      // включении иногда сигнал от кнопки отключения питания приходит быстрее, чем
                                      // программа успевает настроить настройки).
};

extern Settings set;
