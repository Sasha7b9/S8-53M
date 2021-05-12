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

static void OnChanged_Calibrator_Mode(bool)
{
    CalibratorMode::Set(CalibratorMode::Get());
}

DEF_CHOICE_3(cCalibrator_Mode, PageService::PageCalibrator::self,
    "����������", "Calibrator",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "�����", "DC",
    "����", "AC",
    "0�", "OV",
    set.service.calibrator, nullptr, OnChanged_Calibrator_Mode, nullptr
)

static void OnPress_Calibrator_Calibrate()
{
    FPGA::state.need_calibration = true;
}

DEF_BUTTON(cCalibrator_Calibrate, PageService::PageCalibrator::self,
    "�����������", "Calibrate",
    "������ ��������� ����������",
    "Running the calibration procedure",
    nullptr, OnPress_Calibrator_Calibrate
)

DEF_PAGE_2(pageCalibrator, PageService::self, NamePage::ServiceCalibrator,
    "����������", "CALIBRATOR",
    "����������� ������������ � ���������� ������������",
    "Item of the calibrator and calibration of an oscillograph",
    cCalibrator_Mode,       // ������ - ���������� - ����������
    cCalibrator_Calibrate,  // ������ - ���������� - �����������
    nullptr, nullptr, nullptr, nullptr
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

DEF_CHOICE_2(cMath_FFT_Enable, PageService::PageMath::PageFFT::self,
    "�����������", "Display",
    "�������� � ��������� ����������� �������",
    "Enables or disables the display of the spectrum",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.math.fft.enabled, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(cMath_FFT_Scale, PageService::PageMath::PageFFT::self,
    "�����", "Scale",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "��������", "Log",
    "��������", "Linear",
    set.math.fft.scale, nullptr, nullptr, nullptr
)

DEF_CHOICE_3(cMath_FFT_Source, PageService::PageMath::PageFFT::self,
    "��������", "Source",
    "����� ��������� ��� ������� �������",
    "Selecting the source for the calculation of the spectrum",
    "����� 1", "Channel 1",
    "����� 2", "Channel 2",
    "����� 1 + 2", "Channel 1 + 2",
    set.math.fft.source, nullptr, nullptr, nullptr
)

DEF_CHOICE_4(cMath_FFT_Window, PageService::PageMath::PageFFT::self,
    "����", "Window",
    "����� ���� ��� ������� �������",
    "Sets the window to calculate the spectrum",
    "�����������", "Rectangle",
    "��������", "Hamming",
    "��������", "Blackman",
    "�����", "Hann",
    set.math.fft.window, nullptr, nullptr, nullptr
)

static bool IsActive_Math_FFT()
{
    return ModeDrawMath::IsDisabled();
}

static void OnPress_Math_FFT()
{
    if (!IsActive_Math_FFT())
    {
        Warnings::ShowWarningBad(Warning::ImpossibleEnableFFT);
    }
}

static bool IsActive_Math_FFT_Limit()
{
    return set.math.fft.scale.IsLog();
}

DEF_CHOICE_3(cMath_FFT_Limit, PageService::PageMath::PageFFT::self,
    "��������", "Range",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "Here you can set the limit of monitoring the power spectrum",
    "-40��", "-40dB",
    "-60��", "-60dB",
    "-80��", "-80dB",
    set.math.fft.max_DB, IsActive_Math_FFT_Limit, nullptr, nullptr
)

DEF_PAGE_6(pageFFT, PageService::PageMath::self, NamePage::MathFFT,
    "������", "SPECTRUM",
    "����������� ������� �������� �������",
    "Mapping the input signal spectrum",
    cMath_FFT_Enable,                                   // ������ - ���������� - ������ - �����������
    cMath_FFT_Scale,                                    // ������ - ���������� - ������ - �����
    cMath_FFT_Source,                                   // ������ - ���������� - ������ - ��������
    cMath_FFT_Window,                                   // ������ - ���������� - ������ - ����
    PageService::PageMath::PageFFT::PageCursors::self,  // ������ - ���������� - ������ - �������
    cMath_FFT_Limit,                                    // ������ - ���������� - ������ - ��������
    IsActive_Math_FFT, OnPress_Math_FFT, nullptr, nullptr
)

static void OnPress_Math_FFT_Cursors_Exit()
{
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(cMath_FFT_Cursors_Exit, PageService::PageMath::PageFFT::PageCursors::self,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, OnPress_Math_FFT_Cursors_Exit, DrawSB_Exit, nullptr
)

static bool IsActive_Math_FFT_Cursors()
{
    return set.math.fft.enabled;
}

static void OnRegSet_Math_FFT_Cursors(int angle)
{
    set.math.fft.pos_cur[set.math.fft.cursor] += (uint8)angle;
    Sound::RegulatorShiftRotate();
}

static void OnPress_Math_FFT_Cursors_Source()
{
    set.math.fft.cursor = (uint8)((set.math.fft.cursor + 1) % 2);
}

static void Draw_Math_FFT_Cursors_Source(int x, int y)
{
    Text((set.math.fft.cursor == 0) ? "1" : "2").Draw(x + 7, y + 5);
}

DEF_SMALL_BUTTON(cMath_FFT_Cursors_Source, PageService::PageMath::PageFFT::PageCursors::self,
    "��������", "Source",
    "����� ��������� ��� ������� �������",
    "Source choice for calculation of a range",
    nullptr, OnPress_Math_FFT_Cursors_Source, Draw_Math_FFT_Cursors_Source, nullptr
)

DEF_PAGE_6(pageCursorsFFT, PageService::PageMath::PageFFT::self, NamePage::SB_MathCursorsFFT,
    "�������", "CURSORS",
    "�������� ������� ��� ��������� ���������� �������",
    "Includes cursors to measure the parameters of the spectrum",
    cMath_FFT_Cursors_Exit,     // ������ - ���������� - ������ - ������� - �����
    cMath_FFT_Cursors_Source,   // ������ - ���������� - ������ - ������� - ��������
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    IsActive_Math_FFT_Cursors, nullptr, nullptr, OnRegSet_Math_FFT_Cursors
)


static void OnPress_Information_Exit()
{
    Display::SetDrawMode(DrawMode::Auto, 0);
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(sbInformation_Exit, PageService::PageCalibrator::self,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
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
    Text(LANG_RU ? "����������" : "INFORMATION").Draw(x, y);
    y += dY;
    Text(LANG_RU ? "������ : �8-53/1" : "Model : S8-53/1").Draw(x, y);
    y += dY;

    Text(LANG_RU ? "����������� �����������:" : "Software:").Draw(x, y);
    y += dY;

    Text((pchar)(LANG_RU ? "������ %s" : "version %s"), NUM_VER).Draw(x, y);

    y += dY;

    Text("CRC32 : %X", HAL_CRC::Calculate()).Draw(x, y, Color::FILL);

    dY = -10;
    Text("��� ��������� ������ ������� � ����������� ������ ������").DrawInCenterRect(0, 190 + dY, 320, 20);
    Text("����� ����������: ���./����. 8-017-270-02-00").DrawInCenterRect(0, 205 + dY, 320, 20);
    Text("������������: e-mail: mnipi-24(@)tut.by, ���. 8-017-270-02-23").DrawInCenterRect(0, 220 + dY, 320, 20);

    Menu::Draw();
    Display::EndFrame();
}

static void OnPress_Information()
{
    PageService::PageInformation::self->OpenAndSetItCurrent();
    Display::SetDrawMode(DrawMode::Hand, Information_Draw);
}

DEF_PAGE_6(pageInformation, PageService::self, NamePage::SB_Information,
    "����������", "INFORMATION",
    "������� �� ����� ����������������� ������ ������������",
    "Displays identification data of the oscilloscope",
    sbInformation_Exit, // ������ - ���������� - �����
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    nullptr, OnPress_Information, nullptr, nullptr
)

const Page *PageService::self = &pageService;
const Page *PageService::PageMath::PageFFT::self = &pageFFT;
const Page *PageService::PageMath::PageFFT::PageCursors::self = &pageCursorsFFT;
const Page *PageService::PageCalibrator::self = &pageCalibrator;
const Page *PageService::PageInformation::self = &pageInformation;
