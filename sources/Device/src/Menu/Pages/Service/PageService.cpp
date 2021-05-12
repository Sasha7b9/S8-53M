// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Timer_.h"
#include "Display/Display.h"
#include "Panel/Panel.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;



static void FuncDraw()
{
    Display::BeginFrame(Color::BACK);

    Text("����������� ����� �������� �������� ������ ����/����.\n������� ����� ������ ������, ���� ����� �� �����.").DrawInRectWithTransfers(30, 110, 300, 200, Color::FILL);

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
    "����� ��������", "Reset settings",
    "����� �������� �� ��������� �� ���������",
    "Reset to default settings",
    nullptr, OnPress_ResetSettings
)

static void OnPress_AutoSearch()
{
    FPGA::StartAutoFind();
};

DEF_BUTTON(bAutoSearch, PageService::self,
    "����� �������", "Find signal",
    "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
    "Sets optimal settings for the oscilloscope signal on channel 1",
    nullptr, OnPress_AutoSearch
)


DEF_CHOICE_2(cSound, PageService::self,
    "����", "Sound",
    "���������/���������� �����",
    "Inclusion/switching off of a sound",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.service.sound_enabled, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(cLang, PageService::self,
    "����", "Language",
    "��������� ������� ���� ����",
    "Allows you to select the menu language",
    "�������", "Russian",
    "����������", "English",
    LANG, nullptr, nullptr, nullptr
)

DEF_TIME(tTime, PageService::self,
    "�����", "Time"
    ,
    "��������� �������� �������.\n������� ������:\n"
    "������ �� ������� ���� \"�����\". ��������� ���� ��������� �������� �������. ��������� ��������� ������ �� �������� ����������, �������������� "
    "�������� ���������� \"�����\", �������� ����, ������, �������, ���, �����, ��� �����. ���������� ������� ������������ �������� ��������. "
    "��������� ����� ��������� ���������� ����������� ��������. ����� �������� ����� \"���������\", ������ � ���������� ����� 0.5 ��� ������ �� ������ "
    "����������. ���� ��������� �������� ������� ��������� � ����������� ������ �������� �������. ������� ���������� ��������� ������ �� ����� ������ �������� "
    "������� � �������� ���� ��������� �������� ������� ��� ���������� ������ �������� �������"
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
    "��� ���� �����", "Mode long �����"
    ,
    "������������� �������� ��� ����������� ������� ������ �����:\n\"����� ������\" - ��������� ������ ������������� � ����,\n\"�����������\" "
    "- �������������� ����������� � ��������� ������ �������������"
    ,
    "Sets the action for long press CLOCK:\n\"Reset trig lvl\" - to set the trigger level to zero, \n\"Auto level\""
    "- Automatically detect and install the trigger level"
    ,
    "����� ������", "Reset trig level",
    "�����������", "Autolevel",
    set.trig.modeLongPressTrig, nullptr, nullptr, nullptr
)

DEF_PAGE_10(pageService, PageMain::self, NamePage::Service,
    "������", "SERVICE",
    "�������������� ���������, ����������, ����� �������, �������������� �������",
    "Additional settings, calibration, signal search, mathematical functions",
    bResetSettings,                     // ������ - ����� ��������
    bAutoSearch,                        // ������ - ����� �������
    *PageService::PageCalibrator::self, // ������ - ����������
    *PageService::PageMath::self,       // ������ - ����������
    *PageService::PageEthernet::self,   // ������ - ETHERNET
    cSound,                             // ������ - ����
    cLang,                              // ������ - ����
    tTime,                              // ������ - �����
    cModeLongPressButtonTrig,           // ������ - ��� ���� �����
    *PageService::PageCalibrator::self, // ������ - ����������
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageService::self = &pageService;
