#include "defines.h"
#include "common/Display/Text_.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <cstdio>


using namespace Primitives;


static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
static uint16 portMask = 0;
static uint16 portGateway = 0;


static void FuncDraw()
{
    Display::BeginFrame(Color::BACK);

    Text("Подтвердите сброс настроек нажатием кнопки ПУСК/СТОП.\nНажмите любую другую кнопку, если сброс не нужен.").DrawInRectWithTransfers(30, 110, 300, 200, Color::FILL);

    Painter::SendFrame();
}

static void OnTimerDraw()
{
    Display::Update();
}

static void OnPress_ResetSettings()
{
    Panel::Disable();
    Display::SetDrawMode(DrawMode::Hand, FuncDraw);
    Timer::Enable(TypeTimer::TimerDrawHandFunction, 100, OnTimerDraw);

    if (Panel::WaitPressingButton() == Key::Start)
    {
        Settings::Load(true);
        FPGA::Init();
    }

    Timer::Disable(TypeTimer::TimerDrawHandFunction);
    Display::SetDrawMode(DrawMode::Auto, 0);
    Panel::Enable();
}

DEF_BUTTON(bResetSettings, PageService::self,
    "Сброс настроек", "Reset settings",
    "Сброс настроек на настройки по умолчанию",
    "Reset to default settings",
    nullptr, OnPress_ResetSettings
)

static void OnPress_AutoSearch()
{
    FPGA::StartAutoFind();
};

DEF_BUTTON(bAutoSearch, PageService::self,
    "Поиск сигнала", "Find signal",
    "Устанавливает оптимальные установки осциллографа для сигнала в канале 1",
    "Sets optimal settings for the oscilloscope signal on channel 1",
    nullptr, OnPress_AutoSearch
)

static void OnChanged_Calibrator_Mode(bool)
{
    CalibratorMode::Set(CalibratorMode::Get());
}

DEF_CHOICE_3(cCalibrator_Mode, PageService::PageCalibrator::self,
    "Калибратор", "Calibrator",
    "Режим работы калибратора",
    "Mode of operation of the calibrator",
    "Перем", "DC",
    "Пост", "AC",
    "0В", "OV",
    set.service.calibrator, nullptr, OnChanged_Calibrator_Mode, nullptr
)

static void OnPress_Calibrator_Calibrate()
{
    FPGA::state.need_calibration = true;
}

DEF_BUTTON(cCalibrator_Calibrate, PageService::PageCalibrator::self,
    "Калибровать", "Calibrate",
    "Запуск процедуры калибровки",
    "Running the calibration procedure",
    nullptr, OnPress_Calibrator_Calibrate
)

DEF_PAGE_2(pageCalibrator, PageService::self, NamePage::ServiceCalibrator,
    "КАЛИБРАТОР", "CALIBRATOR",
    "Управлением калибратором и калибровка осциллографа",
    "Item of the calibrator and calibration of an oscillograph",
    cCalibrator_Mode,       // СЕРВИС - КАЛИБРАТОР - Калибратор
    cCalibrator_Calibrate,  // СЕРВИС - КАЛИБРАТОР - Калибровать
    nullptr, nullptr, nullptr, nullptr
)

DEF_SMALL_BUTTON(sbMath_Function_Exit, PageService::PageMath::PageFunction::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, nullptr, DrawSB_Exit, nullptr
)

static void Draw_Math_Function_ModeDraw_Disable(int x, int y)
{
    Text(LANG_RU ? "Вык" : "Dis").Draw(x + 2 + (LANG_RU ? 2 : 0), y + 5);
}

static void Draw_Math_Function_ModeDraw_Separate(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
    HLine().Draw(y + 9, x + 3, x + 16);
    HLine().Draw(y + 10, x + 3, x + 16);
}

static void Draw_Math_Function_ModeDraw_Together(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
}

static const arrayHints hintsMath_Function_ModeDraw =
{
    {Draw_Math_Function_ModeDraw_Disable,  "Вывод математической функции отключён",
                                            "The conclusion of mathematical function is disconnected"},
    {Draw_Math_Function_ModeDraw_Separate, "Сигналы и математическая функция выводятся в разных окнах",
                                            "Signals and mathematical function are removed in different windows"},
    {Draw_Math_Function_ModeDraw_Together, "Сигналы и математическая функция выводятся в одном окне",
                                            "Signals and mathematical function are removed in one window"}
};

static void OnPress_Math_Function_ModeDraw()
{
    if (SettingsMath::IsEnbaledFFT())
    {
        Display::ShowWarningBad(Warning::ImpossibleEnableMathFunction);
    }
    else
    {
        GF::CircleIncrease<int8>((int8 *)&set.math.modeDraw, 0, 2);
    }
}

static void Draw_Math_Function_ModeDraw(int x, int y)
{
    static const pFuncVII funcs[3] =
    {
        Draw_Math_Function_ModeDraw_Disable,
        Draw_Math_Function_ModeDraw_Separate,
        Draw_Math_Function_ModeDraw_Together
    };
    funcs[ModeDrawMath::Get()](x, y);
}

DEF_SMALL_BUTTON(sbMath_Function_ModeDraw, PageService::PageMath::PageFunction::self,
    "Экран", "Display",
    "Выбирает режим отображения математического сигнала",
    "Chooses the mode of display of a mathematical signal",
    nullptr, OnPress_Math_Function_ModeDraw, Draw_Math_Function_ModeDraw, &hintsMath_Function_ModeDraw
)

static void OnPress_Math_Function()
{
    if (SettingsMath::IsEnbaledFFT())
    {
        Display::ShowWarningBad(Warning::ImpossibleEnableMathFunction);
    }
}

static bool IsActive_Math_Function()
{
    return !SettingsMath::IsEnbaledFFT();
}

static void OnRegSet_Math_Function(int delta)
{
    if (ModeDrawMath::IsDisabled())
    {
        return;
    }

    if (ModeRegSet::IsRShift())
    {
        int16 prevRShift = RShift::GetMath();
        int16 rShift = prevRShift;
        if (delta > 0)
        {
            if (rShift < RShiftMax)
            {
                rShift += 4 * STEP_RSHIFT;
                LIMIT_ABOVE(rShift, RShiftMax);
                if (prevRShift < RShiftZero && rShift > RShiftZero)
                {
                    rShift = RShiftZero;
                }
                Sound::RegulatorShiftRotate();
                set.math.rShift = rShift;
            }
        }
        else if (delta < 0)
        {
            if (rShift > RShiftMin)
            {
                rShift -= 4 * STEP_RSHIFT;
                LIMIT_BELOW(rShift, RShiftMin);
                if (prevRShift > RShiftZero && rShift < RShiftZero)
                {
                    rShift = RShiftZero;
                }
                Sound::RegulatorShiftRotate();
                set.math.rShift = rShift;
            }
        }
    }
    if (ModeRegSet::IsRange())
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = RSHIFT_2_ABS(RShift::GetMath(), set.math.range);

        if (sum > 2)
        {
            if (set.math.range < Range::Count - 1)
            {
                ++set.math.range;
                set.math.rShift = (int16)RShift::ToRel(rShiftAbs, set.math.range);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (set.math.range > 0)
            {
                --set.math.range;
                set.math.rShift = (int16)RShift::ToRel(rShiftAbs, set.math.range);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
    }
}

static void OnPress_Math_Function_Type()
{
    GF::CircleIncrease<int8>((int8 *)&set.math.func, 0, 1);
}

static void Draw_Math_Function_Type_Sum(int x, int y)
{
    HLine().Draw(y + 9, x + 4, x + 14);
    HLine().Draw(x + 9, y + 4, y + 14);
}

static void Draw_Math_Function_Type_Mul(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::MATH_FUNC_MUL).Draw4SymbolsInRect(x + 4, y + 3);
    Font::Set(TypeFont::S8);
}

static void Draw_Math_Function_Type(int x, int y)
{
    const pFuncVII funcs[2] = { Draw_Math_Function_Type_Sum, Draw_Math_Function_Type_Mul };
    funcs[Function::Get()](x, y);
}

static const arrayHints hintsMath_Function_Type =
{
    { Draw_Math_Function_Type_Sum,      "Сложение",     "Addition"       },
    { Draw_Math_Function_Type_Mul,      "Умножение",    "Multiplication" }
};

DEF_SMALL_BUTTON(sbMath_Function_Type, PageService::PageMath::PageFunction::self,
    "Вид", "Type",
    "Выбор математической функции",
    "Choice of mathematical function",
    nullptr, OnPress_Math_Function_Type, Draw_Math_Function_Type, &hintsMath_Function_Type
)

static void OnPress_Math_Function_ModeRegSet()
{
    GF::CircleIncrease<int8>((int8 *)&set.math.modeRegSet, 0, 1);
}

static void Draw_Math_Function_ModeRegSet_Range(int x, int y)
{
    Char(LANG_RU ? 'M' : 'S').Draw(x + 7, y + 5);
}

static void Draw_Math_Function_ModeRegSet_RShift(int x, int y)
{
    Text(LANG_RU ? "См" : "Shif").Draw(x + 5 - (LANG_EN ? 3 : 0), y + 5);
}

static void Draw_Math_Function_ModeRegSet(int x, int y)
{
    const pFuncVII funcs[2] = { Draw_Math_Function_ModeRegSet_Range, Draw_Math_Function_ModeRegSet_RShift };
    funcs[ModeRegSet::Get()](x, y);
}

static const arrayHints hintsMath_Function_ModeRegSet =
{
    {Draw_Math_Function_ModeRegSet_Range,  "Управление масштабом", "Management of scale"},
    {Draw_Math_Function_ModeRegSet_RShift, "Управление смещением", "Management of shift"}
};

DEF_SMALL_BUTTON(sbMath_Function_ModeRegSet, PageService::PageMath::PageFunction::self,
    "Режим ручки УСТАНОВКА", "Mode regulator SET",
    "Выбор режима ручки УСТАНОВКА - управление масштабом или смещением",
    "Choice mode regulcator УСТАНОВКА - management of scale or shift",
    nullptr, OnPress_Math_Function_ModeRegSet, Draw_Math_Function_ModeRegSet, &hintsMath_Function_ModeRegSet
)

static void OnPress_Math_Function_RangeA()
{
    set.math.range = set.chan[ChA].range;
    set.math.multiplier = set.chan[ChA].divider;
}

static void Draw_Math_Function_RangeA(int x, int y)
{
    Char('1').Draw(x + 8, y + 5);
}

DEF_SMALL_BUTTON(sbMath_Function_RangeA, PageService::PageMath::PageFunction::self,
    "Масштаб 1-го канала", "Scale of the 1st channel",
    "Берёт масштаб для математического сигнала из первого канала",
    "Takes scale for a mathematical signal from the first channel",
    nullptr, OnPress_Math_Function_RangeA, Draw_Math_Function_RangeA, nullptr
)

static void OnPress_Math_Function_RangeB()
{
    set.math.range = set.chan[ChB].range;
    set.math.multiplier = set.chan[ChB].divider;
}

static void Draw_Math_Function_RangeB(int x, int y)
{
    Char('2').Draw(x + 8, y + 5);
}

DEF_SMALL_BUTTON(sbMath_Function_RangeB, PageService::PageMath::PageFunction::self,
    "Масштаб 2-го канала", "Scale of the 2nd channel",
    "Берёт масштаб для математического сигнала из второго канала",
    "Takes scale for a mathematical signal from the second channel",
    nullptr, OnPress_Math_Function_RangeB, Draw_Math_Function_RangeB, nullptr
)

DEF_PAGE_6(pageMathFunction, PageService::PageMath::self, NamePage::SB_MathFunction,
    "ФУНКЦИЯ", "FUNCTION",
    "Установка и выбор математической функции - сложения или умножения",
    "Installation and selection of mathematical functions - addition or multiplication",
    sbMath_Function_Exit,       // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ - Выход
    sbMath_Function_ModeDraw,   // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ - Экран
    sbMath_Function_Type,       // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ - Вид
    sbMath_Function_ModeRegSet, // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ - Режим ручки УСТАНОВКА
    sbMath_Function_RangeA,     // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ - Масштаб 1-го канала
    sbMath_Function_RangeB,     // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ - Масштаб 2-го канала    
    IsActive_Math_Function, OnPress_Math_Function, nullptr, OnRegSet_Math_Function
);

DEF_PAGE_2(pageMath, PageService::self, NamePage::Math,
    "МАТЕМАТИКА", "MATH",
    "Математические функции и БПФ",
    "Mathematical functions and FFT",
    *PageService::PageMath::PageFunction::self,     // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ
    *PageService::PageMath::PageFFT::self,          // СЕРВИС - МАТЕМАТИКА - СПЕКТР
    nullptr, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(cSound, PageService::self,
    "Звук", "Sound",
    "Включение/выключение звука",
    "Inclusion/switching off of a sound",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.service.sound_enabled, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(cLang, PageService::self,
    "Язык", "Language",
    "Позволяет выбрать язык меню",
    "Allows you to select the menu language",
    "Русский", "Russian",
    "Английский", "English",
    LANG, nullptr, nullptr, nullptr
)

DEF_TIME(tTime, PageService::self,
    "Время", "Time"
    ,
    "Установка текущего времени.\nПорядок работы:\n"
    "Нажать на элемент меню \"Время\". Откроется меню установки текущего времени. Короткими нажатиями кнопки на цифровой клавиатуре, соответсвующей "
    "элементу управления \"Время\", выделить часы, минуты, секунды, год, месяц, или число. Выделенный элемент обозначается мигающей областью. "
    "Вращением ручки УСТАНОВКА установить необходимое значение. Затем выделить пункт \"Сохранить\", нажать и удреживать более 0.5 сек кнопку на панели "
    "управления. Меню установки текущего временя закроется с сохранением нового текущего времени. Нажатие длительное удержание кнопки на любом другом элементе "
    "приведёт к закрытию меню установки текущего времени без сохранения нового текущего времени"
    ,
    "Setting the current time. \nPoryadok work:\n"
    "Click on the menu item \"Time\".The menu set the current time.By briefly pressing the button on the numeric keypad of conformity "
    "Item \"Time\", highlight the hours, minutes, seconds, year, month, or a number.The selected item is indicated by a flashing area. "
    "Turn the setting knob to set the desired value. Then highlight \"Save\", press and udrezhivat more than 0.5 seconds, the button on the panel "
    "Item. Menu Setting the current time will be closed to the conservation of the new current time. Pressing a button on the prolonged retention of any other element "
    "will lead to the closure of the current time setting menu without saving the new current time"
    ,
    dServicetime, hours, minutes, secondes, month, day, year
)

DEF_CHOICE_2(cModeLongPressButtonTrig, PageService::self,
    "Реж длит СИНХР", "Mode long СИНХР"
    ,
    "Устанавливает действия для длительного нажатия кнопки СИНХР:\n\"Сброс уровня\" - установка уровня синхронизации в ноль,\n\"Автоуровень\" "
    "- автоматическое определение и установка уровня синхронизации"
    ,
    "Sets the action for long press CLOCK:\n\"Reset trig lvl\" - to set the trigger level to zero, \n\"Auto level\""
    "- Automatically detect and install the trigger level"
    ,
    "Сброс уровня", "Reset trig level",
    "Автоуровень", "Autolevel",
    set.trig.modeLongPressTrig, nullptr, nullptr, nullptr
)

DEF_PAGE_10(pageService, PageMain::self, NamePage::Service,
    "СЕРВИС", "SERVICE",
    "Дополнительные настройки, калибровка, поиск сигнала, математические функции",
    "Additional settings, calibration, signal search, mathematical functions",
    bResetSettings,                     // СЕРВИС - Сброс настроек
    bAutoSearch,                        // СЕРВИС - Поиск сигнала
    *PageService::PageCalibrator::self, // СЕРВИС - КАЛИБРАТОР
    *PageService::PageMath::self,       // СЕРВИС - МАТЕМАТИКА
    *PageService::PageEthernet::self,   // СЕРВИС - ETHERNET
    cSound,                             // СЕРВИС - Звук
    cLang,                              // СЕРВИС - Язык
    tTime,                              // СЕРВИС - Время
    cModeLongPressButtonTrig,           // СЕРВИС - Реж длит СИНХР
    *PageService::PageCalibrator::self, // СЕРВИС - ИНФОРМАЦИЯ
    nullptr, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(cMath_FFT_Enable, PageService::PageMath::PageFFT::self,
    "Отображение", "Display",
    "Включает и выключает отображение спектра",
    "Enables or disables the display of the spectrum",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.math.enableFFT, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(cMath_FFT_Scale, PageService::PageMath::PageFFT::self,
    "Шкала", "Scale",
    "Задаёт масштаб вывода спектра - линейный или логарифмический",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "Логарифм", "Log",
    "Линейная", "Linear",
    set.math.scaleFFT, nullptr, nullptr, nullptr
)

DEF_CHOICE_3(cMath_FFT_Source, PageService::PageMath::PageFFT::self,
    "Источник", "Source",
    "Выбор источника для расчёта спектра",
    "Selecting the source for the calculation of the spectrum",
    "Канал 1", "Channel 1",
    "Канал 2", "Channel 2",
    "Канал 1 + 2", "Channel 1 + 2",
    set.math.sourceFFT, nullptr, nullptr, nullptr
)

DEF_CHOICE_4(cMath_FFT_Window, PageService::PageMath::PageFFT::self,
    "Окно", "Window",
    "Задаёт окно для расчёта спектра",
    "Sets the window to calculate the spectrum",
    "Прямоугольн", "Rectangle",
    "Хэмминга", "Hamming",
    "Блэкмена", "Blackman",
    "Ханна", "Hann",
    set.math.windowFFT, nullptr, nullptr, nullptr
)

static bool IsActive_Math_FFT()
{
    return ModeDrawMath::IsDisabled();
}

static void OnPress_Math_FFT()
{
    if (!IsActive_Math_FFT())
    {
        Display::ShowWarningBad(Warning::ImpossibleEnableFFT);
    }
}

static bool IsActive_Math_FFT_Limit()
{
    return ScaleFFT::IsLog();
}

DEF_CHOICE_3(cMath_FFT_Limit, PageService::PageMath::PageFFT::self,
    "Диапазон", "Range",
    "Здесь можно задать предел наблюдения за мощностью спектра",
    "Here you can set the limit of monitoring the power spectrum",
    "-40дБ", "-40dB",
    "-60дБ", "-60dB",
    "-80дБ", "-80dB",
    set.math.fftMaxDB, IsActive_Math_FFT_Limit, nullptr, nullptr
)

DEF_PAGE_6(pageFFT, PageService::PageMath::self, NamePage::MathFFT,
    "СПЕКТР", "SPECTRUM",
    "Отображение спектра входного сигнала",
    "Mapping the input signal spectrum",
    cMath_FFT_Enable,                                   // СЕРВИС - МАТЕМАТИКА - СПЕКТР - Отображение
    cMath_FFT_Scale,                                    // СЕРВИС - МАТЕМАТИКА - СПЕКТР - Шкала
    cMath_FFT_Source,                                   // СЕРВИС - МАТЕМАТИКА - СПЕКТР - Источник
    cMath_FFT_Window,                                   // СЕРВИС - МАТЕМАТИКА - СПЕКТР - Окно
    PageService::PageMath::PageFFT::PageCursors::self,  // СЕРВИС - МАТЕМАТИКА - СПЕКТР - КУРСОРЫ
    cMath_FFT_Limit,                                    // СЕРВИС - МАТЕМАТИКА - СПЕКТР - Диапазон
    IsActive_Math_FFT, OnPress_Math_FFT, nullptr, nullptr
)

static void OnPress_Math_FFT_Cursors_Exit()
{
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(cMath_FFT_Cursors_Exit, PageService::PageMath::PageFFT::PageCursors::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, OnPress_Math_FFT_Cursors_Exit, DrawSB_Exit, nullptr
)

static bool IsActive_Math_FFT_Cursors()
{
    return SettingsMath::IsEnbaledFFT();
}

static void OnRegSet_Math_FFT_Cursors(int angle)
{
    set.math.posCur[SettingsMath::CursorFFT()] += (uint8)angle;
    Sound::RegulatorShiftRotate();
}

static void OnPress_Math_FFT_Cursors_Source()
{
    set.math.cursorFFT = (uint8)((SettingsMath::CursorFFT() + 1) % 2);
}

static void Draw_Math_FFT_Cursors_Source(int x, int y)
{
    Text((SettingsMath::CursorFFT() == 0) ? "1" : "2").Draw(x + 7, y + 5);
}

DEF_SMALL_BUTTON(cMath_FFT_Cursors_Source, PageService::PageMath::PageFFT::PageCursors::self,
    "Источник", "Source",
    "Выбор источника для расчёта спектра",
    "Source choice for calculation of a range",
    nullptr, OnPress_Math_FFT_Cursors_Source, Draw_Math_FFT_Cursors_Source, nullptr
)

DEF_PAGE_6(pageCursorsFFT, PageService::PageMath::PageFFT::self, NamePage::SB_MathCursorsFFT,
    "КУРСОРЫ", "CURSORS",
    "Включает курсоры для измерения параметров спектра",
    "Includes cursors to measure the parameters of the spectrum",
    cMath_FFT_Cursors_Exit,     // СЕРВИС - МАТЕМАТИКА - СПЕКТР - КУРСОРЫ - Выход
    cMath_FFT_Cursors_Source,   // СЕРВИС - МАТЕМАТИКА - СПЕКТР - КУРСОРЫ - Источник
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    IsActive_Math_FFT_Cursors, nullptr, nullptr, OnRegSet_Math_FFT_Cursors
)

static void OnChanged_Ethernet_Enable(bool)
{
    Display::ShowWarningGood(Warning::NeedRebootDevice2);
    Display::ShowWarningGood(Warning::NeedRebootDevice1);
}

DEF_CHOICE_2(cEthernet_Enable, PageService::PageEthernet::self,
    "Ethernet", "Ethernet"
    ,
    "Чтобы задействовать ethernet, выберите \"Включено\" и выключите прибор.\n"
    "Чтобы отключить ethernet, выберите \"Отключено\" и выключите прибор."
    ,
    "To involve ethernet, choose \"Included\" and switch off the device.\n"
    "To disconnect ethernet, choose \"Disconnected\" and switch off the device."
    ,
    "Включено", "Included",
    "Отключено", "Disconnected",
    set.LAN.enabled, nullptr, OnChanged_Ethernet_Enable, nullptr
)

DEF_IPADDRESS(ipEthernet_IP, PageService::PageEthernet::self,
    "IP адрес", "IP-address",
    "Установка IP адреса",
    "Set of IP-address",
    IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3, set.LAN.port, OnChanged_Ethernet_Enable
)


DEF_IPADDRESS(ipEthernet_Mask, PageService::PageEthernet::self,
    "Маска подсети", "Network mask",
    "Установка маски подсети",
    "Set of network mask",
    NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3, portMask, OnChanged_Ethernet_Enable
)

DEF_IPADDRESS(ipEthernet_Gateway, PageService::PageEthernet::self,
    "Шлюз", "Gateway",
    "Установка адреса основного шлюза",
    "Set of gateway address",
    GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3, portGateway, OnChanged_Ethernet_Enable
)

DEF_MACADDRESS(macEthernet_MAC, PageService::PageEthernet::self,
    "Физ адрес", "MAC-address",
    "Установка физического адреса",
    "Set of MAC-address",
    MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5, OnChanged_Ethernet_Enable
)

DEF_PAGE_5(pageEthernet, PageService::self, NamePage::ServiceEthernet,
    "ETHERNET", "ETHERNET",
    "Настройки ethernet",
    "Settings of ethernet",
    cEthernet_Enable,       // СЕРВИС - ETHERNET - Ethernet
    ipEthernet_IP,          // СЕРВИС - ETHERNET - IP адрес
    ipEthernet_Mask,        // СЕРВИС - ETHERNET - Маска подсети
    ipEthernet_Gateway,     // СЕРВИС - ETHERNET - Шлюз
    macEthernet_MAC,        // СЕРВИС - ETHERNET - Физ адрес
    nullptr, nullptr, nullptr, nullptr
)

static void OnPress_Information_Exit()
{
    Display::SetDrawMode(DrawMode::Auto, 0);
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(sbInformation_Exit, PageService::PageCalibrator::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, OnPress_Information_Exit, DrawSB_Exit, nullptr
)

static void Information_Draw()
{
    Display::BeginFrame(Color::BLACK);
    int x = 100;
    int dY = 20;
    int y = 20;
    Rectangle(319, 239).Draw(0, 0, Color::FILL);
    y += dY;
    Text(LANG_RU ? "ИНФОРМАЦИЯ" : "INFORMATION").Draw(x, y);
    y += dY;
    Text(LANG_RU ? "Модель : С8-53/1" : "Model : S8-53/1").Draw(x, y);
    y += dY;

    char buffer[100];

    Text(LANG_RU ? "Программное обеспечение:" : "Software:").Draw(x, y);
    y += dY;
    std::sprintf(buffer, (pchar)(LANG_RU ? "версия %s" : "version %s"), NUM_VER);
    Text(buffer).Draw(x, y);
    y += dY;

    Text(String("CRC32 : %X", HAL_CRC::Calculate())).Draw(x, y, Color::FILL);

    dY = -10;
    Text("Для получения помощи нажмите и удерживайте кнопку ПОМОЩЬ").DrawInCenterRect(0, 190 + dY, 320, 20);
    Text("Отдел маркетинга: тел./факс. 8-017-270-02-00").DrawInCenterRect(0, 205 + dY, 320, 20);
    Text("Разработчики: e-mail: mnipi-24(@)tut.by, тел. 8-017-270-02-23").DrawInCenterRect(0, 220 + dY, 320, 20);

    Menu::Draw();
    Display::EndFrame();
}

static void OnPress_Information()
{
    PageService::PageInformation::self->OpenAndSetItCurrent();
    Display::SetDrawMode(DrawMode::Hand, Information_Draw);
}

DEF_PAGE_6(pageInformation, PageService::self, NamePage::SB_Information,
    "ИНФОРМАЦИЯ", "INFORMATION",
    "Выводит на экран идентификационные данные осциллографа",
    "Displays identification data of the oscilloscope",
    sbInformation_Exit, // СЕРВИС - ИНФОРМАЦИЯ - Выход
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    nullptr, OnPress_Information, nullptr, nullptr
)

const Page *PageService::self = &pageService;
const Page *PageService::PageEthernet::self = &pageEthernet;
const Page *PageService::PageMath::self = &pageMath;
const Page *PageService::PageMath::PageFunction::self = &pageMathFunction;
const Page *PageService::PageMath::PageFFT::self = &pageFFT;
const Page *PageService::PageMath::PageFFT::PageCursors::self = &pageCursorsFFT;
const Page *PageService::PageCalibrator::self = &pageCalibrator;
const Page *PageService::PageInformation::self = &pageInformation;
