// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Display/Warnings.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <cstdio>


static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;



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

    Text(LANG_RU ? "Программное обеспечение:" : "Software:").Draw(x, y);
    y += dY;

    Text((pchar)(LANG_RU ? "версия %s" : "version %s"), NUM_VER).Draw(x, y);

    y += dY;

    Text("CRC32 : %X", HAL_CRC::Calculate()).Draw(x, y, Color::FILL);

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
const Page *PageService::PageCalibrator::self = &pageCalibrator;
const Page *PageService::PageInformation::self = &pageInformation;
