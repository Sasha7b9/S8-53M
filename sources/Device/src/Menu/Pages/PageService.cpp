#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "common/Utils/Math_c.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Pages/PageService.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <cstdio>


using namespace Primitives;


extern const Page pService;


extern const Button       bResetSettings;               // ������ - ����� ��������
static void        OnPress_ResetSettings();
static void FuncDraw();
static void OnTimerDraw();
extern const Button       bAutoSearch;                  // ������ - ����� �������
static void        OnPress_AutoSearch();
extern const Page        ppCalibrator;                  // ������ - ����������
extern const Choice       cCalibrator_Mode;             // ������ - ���������� - ����������
static void      OnChanged_Calibrator_Mode(bool active);
extern const Button       cCalibrator_Calibrate;        // ������ - ���������� - �����������
static void        OnPress_Calibrator_Calibrate();
extern const Page        ppMath;                        // ������ - ����������
extern const Page       pppMath_Function;               // ������ - ���������� - �������
static bool       IsActive_Math_Function();
static void        OnPress_Math_Function();
static void       OnRegSet_Math_Function(int delta);
extern const SmallButton sbMath_Function_Exit;          // ������ - ���������� - ������� - �����
extern const SmallButton sbMath_Function_ModeDraw;      // ������ - ���������� - ������� - �����
static void        OnPress_Math_Function_ModeDraw();
static void           Draw_Math_Function_ModeDraw(int x, int y);
static void           Draw_Math_Function_ModeDraw_Disable(int x, int y);
static void           Draw_Math_Function_ModeDraw_Separate(int x, int y);
static void           Draw_Math_Function_ModeDraw_Together(int x, int y);
extern const SmallButton sbMath_Function_Type;          // ������ - ���������� - ������� - ���
static void        OnPress_Math_Function_Type();
static void           Draw_Math_Function_Type(int x, int y);
static void           Draw_Math_Function_Type_Sum(int x, int y);
static void           Draw_Math_Function_Type_Mul(int x, int y);
extern const SmallButton sbMath_Function_ModeRegSet;    // ������ - ���������� - ������� - ����� ����� ���������
static void        OnPress_Math_Function_ModeRegSet();
static void           Draw_Math_Function_ModeRegSet(int x, int y);
static void           Draw_Math_Function_ModeRegSet_Range(int x, int y);
static void           Draw_Math_Function_ModeRegSet_RShift(int x, int y);
extern const SmallButton sbMath_Function_RangeA;        // ������ - ���������� - ������� - ������� 1-�� ������
static void        OnPress_Math_Function_RangeA();
static void           Draw_Math_Function_RangeA(int x, int y);
extern const SmallButton sbMath_Function_RangeB;        // ������ - ���������� - ������� - ������� 2-�� ������
static void        OnPress_Math_Function_RangeB();
static void           Draw_Math_Function_RangeB(int x, int y);
extern const Page       pppMath_FFT;                    // ������ - ���������� - ������
static void        OnPress_Math_FFT();
static bool       IsActive_Math_FFT();
extern const Choice       cMath_FFT_Enable;             // ������ - ���������� - ������ - �����������
extern const Choice       cMath_FFT_Scale;              // ������ - ���������� - ������ - �����
extern const Choice       cMath_FFT_Source;             // ������ - ���������� - ������ - ��������
extern const Choice       cMath_FFT_Window;             // ������ - ���������� - ������ - ����
extern const Page      ppppMath_FFT_Cursors;            // ������ - ���������� - ������ - �������
static bool       IsActive_Math_FFT_Cursors();
static void       OnRegSet_Math_FFT_Cursors(int angle);
extern const SmallButton  cMath_FFT_Cursors_Exit;        // ������ - ���������� - ������ - ������� - �����
static void        OnPress_Math_FFT_Cursors_Exit();
extern const SmallButton  cMath_FFT_Cursors_Source;      // ������ - ���������� - ������ - ������� - ��������
static void        OnPress_Math_FFT_Cursors_Source();
static void           Draw_Math_FFT_Cursors_Source(int x, int y);
extern const Choice       cMath_FFT_Limit;              // ������ - ���������� - ������ - ��������
static bool       IsActive_Math_FFT_Limit();
extern const Page        ppEthernet;                    // ������ - ETHERNET
extern const Choice       cEthernet_Enable;             // ������ - ETHERNET - Ethernet
static void      OnChanged_Ethernet_Enable(bool active);
extern const IPaddress   ipEthernet_IP;                 // ������ - ETHERNET - IP �����
extern const IPaddress   ipEthernet_Mask;               // ������ - ETHERNET - ����� �������
extern const IPaddress   ipEthernet_Gateway;            // ������ - ETHERNET - ����
extern const MACaddress macEthernet_MAC;                // ������ - ETHERNET - ��� �����
extern const Choice       cSound;                       // ������ - ����
extern const Choice       cLang;                        // ������ - ����
extern const TimeItem     tTime;                        // ������ - �����
extern const Choice       cModeLongPressButtonTrig;     // ������ - ��� ���� �����
extern const Page        ppInformation;                 // ������ - ����������
static void        OnPress_Information();
static void Information_Draw();
extern const SmallButton sbInformation_Exit;             // ������ - ���������� - �����
static void        OnPress_Information_Exit();

extern const Page mainPage;

// ������ //////////////////////////
DEF_PAGE_10(pService, mainPage, NamePage::Service,
    "������", "SERVICE",
    "�������������� ���������, ����������, ����� �������, �������������� �������",
    "Additional settings, calibration, signal search, mathematical functions",
    bResetSettings,             // ������ - ����� ��������
    bAutoSearch,                // ������ - ����� �������
    ppCalibrator,               // ������ - ����������
    ppMath,                     // ������ - ����������
    ppEthernet,                 // ������ - ETHERNET
    cSound,                     // ������ - ����
    cLang,                      // ������ - ����
    tTime,                      // ������ - �����
    cModeLongPressButtonTrig,   // ������ - ��� ���� �����
    ppInformation,              // ������ - ����������
    nullptr, nullptr, nullptr, nullptr
);

// ������ - ����� �������� ---------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(bResetSettings, pService,
    "����� ��������", "Reset settings",
    "����� �������� �� ��������� �� ���������",
    "Reset to default settings",
    nullptr, OnPress_ResetSettings
)

static void OnPress_ResetSettings(void)
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

static void FuncDraw(void)
{
    Painter::BeginScene(COLOR_BACK);

    Painter::DrawTextInRectWithTransfersC(30, 110, 300, 200, "����������� ����� �������� �������� ������ ����/����.\n"
                                         "������� ����� ������ ������, ���� ����� �� �����.", COLOR_FILL);

    Painter::EndScene();
}

static void OnTimerDraw(void)
{
    Display::Update();
}


// ������ - ����� ������� ----------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(bAutoSearch, pService,
    "����� �������", "Find signal",
    "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
    "Sets optimal settings for the oscilloscope signal on channel 1",
    nullptr, OnPress_AutoSearch
)

static void OnPress_AutoSearch(void)
{
    FPGA::StartAutoFind();
};

// ������ - ���������� /////////////
DEF_PAGE_2(ppCalibrator, pService, NamePage::ServiceCalibrator,
    "����������", "CALIBRATOR",
    "����������� ������������ � ���������� ������������",
    "Item of the calibrator and calibration of an oscillograph",
    cCalibrator_Mode,       // ������ - ���������� - ����������
    cCalibrator_Calibrate,  // ������ - ���������� - �����������
    nullptr, nullptr, nullptr, nullptr
)

// ������ - ���������� - ���������� ------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(cCalibrator_Mode, ppCalibrator,
    "����������",  "Calibrator",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "�����", "DC",
    "����",  "AC",
    "0�",    "OV",
    CALIBRATOR, nullptr, OnChanged_Calibrator_Mode, nullptr
)

static void OnChanged_Calibrator_Mode(bool)
{
    FPGA::SetCalibratorMode(CALIBRATOR);
}

// ������ - ���������� - ����������� -----------------------------------------------------------------------------------------------------------------
DEF_BUTTON(cCalibrator_Calibrate, ppCalibrator,
    "�����������", "Calibrate",
    "������ ��������� ����������",
    "Running the calibration procedure",
    nullptr, OnPress_Calibrator_Calibrate
)

static void OnPress_Calibrator_Calibrate(void)
{
    gStateFPGA.needCalibration = true;
}

// ������ - ���������� /////////////
DEF_PAGE_2(ppMath, pService, NamePage::Math,
    "����������", "MATH",
    "�������������� ������� � ���",
    "Mathematical functions and FFT",
    pppMath_Function,     // ������ - ���������� - �������
    pppMath_FFT,          // ������ - ���������� - ������
    nullptr, nullptr, nullptr, nullptr
)

// ������ - ���������� - ������� ///
DEF_PAGE_6(pppMath_Function, ppMath, NamePage::SB_MathFunction, 
    "�������", "FUNCTION",
    "��������� � ����� �������������� ������� - �������� ��� ���������",
    "Installation and selection of mathematical functions - addition or multiplication",
    sbMath_Function_Exit,       // ������ - ���������� - ������� - �����
    sbMath_Function_ModeDraw,   // ������ - ���������� - ������� - �����
    sbMath_Function_Type,       // ������ - ���������� - ������� - ���
    sbMath_Function_ModeRegSet, // ������ - ���������� - ������� - ����� ����� ���������
    sbMath_Function_RangeA,     // ������ - ���������� - ������� - ������� 1-�� ������
    sbMath_Function_RangeB,     // ������ - ���������� - ������� - ������� 2-�� ������    
    IsActive_Math_Function, OnPress_Math_Function, nullptr, OnRegSet_Math_Function
);

void *PageService::Math::Function::GetPointer()
{
    return (void *)&pppMath_Function;
}

static bool IsActive_Math_Function(void)
{
    return !ENABLED_FFT;
}

static void OnPress_Math_Function(void)
{
    if (ENABLED_FFT)
    {
        Display::ShowWarningBad(Warning::ImpossibleEnableMathFunction);
    }
}

static void OnRegSet_Math_Function(int delta)
{
    if (DISABLED_DRAW_MATH)
    {
        return;
    }

    if (MATH_MODE_REG_SET_IS_RSHIFT)
    {
        int16 prevRShift = SET_RSHIFT_MATH;
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
                SET_RSHIFT_MATH = rShift;
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
                SET_RSHIFT_MATH = rShift;
            }
        }
    }
    if (MATH_MODE_REG_SET_IS_RANGE)
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = RSHIFT_2_ABS(SET_RSHIFT_MATH, SET_RANGE_MATH);

        if (sum > 2)
        {
            if (SET_RANGE_MATH < Range::Count - 1)
            {
                ++SET_RANGE_MATH;
                SET_RSHIFT_MATH = (int16)Math::RShift2Rel(rShiftAbs, SET_RANGE_MATH);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (SET_RANGE_MATH > 0)
            {
                --SET_RANGE_MATH;
                SET_RSHIFT_MATH = (int16)Math::RShift2Rel(rShiftAbs, SET_RANGE_MATH);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
    }
}

// ������ - ���������� - ������� - ����� -------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(sbMath_Function_Exit, pppMath_Function,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, nullptr, DrawSB_Exit, nullptr
)

// ������ - ���������� - ������� - ����� -------------------------------------------------------------------------------------------------------------
static const arrayHints hintsMath_Function_ModeDraw =
{
    {Draw_Math_Function_ModeDraw_Disable,  "����� �������������� ������� ��������",
                                            "The conclusion of mathematical function is disconnected"},
    {Draw_Math_Function_ModeDraw_Separate, "������� � �������������� ������� ��������� � ������ �����",
                                            "Signals and mathematical function are removed in different windows"},
    {Draw_Math_Function_ModeDraw_Together, "������� � �������������� ������� ��������� � ����� ����",
                                            "Signals and mathematical function are removed in one window"}
};

DEF_SMALL_BUTTON(sbMath_Function_ModeDraw, pppMath_Function,
    "�����", "Display",
    "�������� ����� ����������� ��������������� �������",
    "Chooses the mode of display of a mathematical signal",
    nullptr, OnPress_Math_Function_ModeDraw, Draw_Math_Function_ModeDraw, &hintsMath_Function_ModeDraw
)

static void OnPress_Math_Function_ModeDraw(void)
{
    if (ENABLED_FFT)
    {
        Display::ShowWarningBad(Warning::ImpossibleEnableMathFunction);
    }
    else
    {
        CircleIncreaseInt8((int8*)&MODE_DRAW_MATH, 0, 2);
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
    funcs[MODE_DRAW_MATH](x, y);
}

static void Draw_Math_Function_ModeDraw_Disable(int x, int y)
{
    Text(LANG_RU ? "���" : "Dis").Draw(x + 2 + (LANG_RU ? 2 : 0), y + 5);
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

// ������ - ���������� - ������� - ��� ---------------------------------------------------------------------------------------------------------------
static const arrayHints hintsMath_Function_Type =
{
    { Draw_Math_Function_Type_Sum,      "��������",     "Addition"       },
    { Draw_Math_Function_Type_Mul,      "���������",    "Multiplication" }
};

DEF_SMALL_BUTTON(sbMath_Function_Type, pppMath_Function,
    "���", "Type",
    "����� �������������� �������",
    "Choice of mathematical function",
    nullptr, OnPress_Math_Function_Type, Draw_Math_Function_Type, &hintsMath_Function_Type
)

static void OnPress_Math_Function_Type(void)
{
    CircleIncreaseInt8((int8*)&MATH_FUNC, 0, 1);
}

static void Draw_Math_Function_Type(int x, int y)
{
    const pFuncVII funcs[2] = {Draw_Math_Function_Type_Sum, Draw_Math_Function_Type_Mul};
    funcs[MATH_FUNC](x, y);
}

static void Draw_Math_Function_Type_Sum(int x, int y)
{
    HLine().Draw(y + 9, x + 4, x + 14);
    HLine().Draw(x + 9, y + 4, y + 14);
}

static void Draw_Math_Function_Type_Mul(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 4, y + 3, SYMBOL_MATH_FUNC_MUL);
    Font::Set(TypeFont::_8);
}

// ������ - ���������� - ������� - ����� ����� ��������� ---------------------------------------------------------------------------------------------
static const arrayHints hintsMath_Function_ModeRegSet =
{
    {Draw_Math_Function_ModeRegSet_Range,  "���������� ���������", "Management of scale"},
    {Draw_Math_Function_ModeRegSet_RShift, "���������� ���������", "Management of shift"}
};

DEF_SMALL_BUTTON(sbMath_Function_ModeRegSet, pppMath_Function,
    "����� ����� ���������", "Mode regulator SET",
    "����� ������ ����� ��������� - ���������� ��������� ��� ���������",
    "Choice mode regulcator ��������� - management of scale or shift",
    nullptr, OnPress_Math_Function_ModeRegSet, Draw_Math_Function_ModeRegSet, &hintsMath_Function_ModeRegSet
)

static void OnPress_Math_Function_ModeRegSet(void)
{
    CircleIncreaseInt8((int8*)&MATH_MODE_REG_SET, 0, 1);
}

static void Draw_Math_Function_ModeRegSet(int x, int y)
{
    const pFuncVII funcs[2] = {Draw_Math_Function_ModeRegSet_Range, Draw_Math_Function_ModeRegSet_RShift};
    funcs[MATH_MODE_REG_SET](x, y);
}

static void Draw_Math_Function_ModeRegSet_Range(int x, int y)
{
    Char(LANG_RU ? 'M' : 'S').Draw(x + 7, y + 5);
}

static void Draw_Math_Function_ModeRegSet_RShift(int x, int y)
{
    Text(LANG_RU ? "��" : "Shif").Draw(x + 5 - (LANG_EN ? 3 : 0), y + 5);
}

// ������ - ���������� - ������� - ������� 1-�� ������ -----------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(sbMath_Function_RangeA, pppMath_Function,
    "������� 1-�� ������", "Scale of the 1st channel",
    "���� ������� ��� ��������������� ������� �� ������� ������",
    "Takes scale for a mathematical signal from the first channel",
    nullptr, OnPress_Math_Function_RangeA, Draw_Math_Function_RangeA, nullptr
)

static void OnPress_Math_Function_RangeA(void)
{
    SET_RANGE_MATH = SET_RANGE_A;
    MATH_MULTIPLIER = SET_DIVIDER_A;
}

static void Draw_Math_Function_RangeA(int x, int y)
{
    Char('1').Draw(x + 8, y + 5);
}

// ������ - ���������� - ������� - ������� 2-�� ������ -----------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(sbMath_Function_RangeB, pppMath_Function,
    "������� 2-�� ������", "Scale of the 2nd channel",
    "���� ������� ��� ��������������� ������� �� ������� ������",
    "Takes scale for a mathematical signal from the second channel",
    nullptr, OnPress_Math_Function_RangeB, Draw_Math_Function_RangeB, nullptr
)

static void OnPress_Math_Function_RangeB(void)
{
    SET_RANGE_MATH = SET_RANGE_B;
    MATH_MULTIPLIER = SET_DIVIDER_B;
}

static void Draw_Math_Function_RangeB(int x, int y)
{
    Char('2').Draw(x + 8, y + 5);
}

// ������ - ���������� - ������ ////
DEF_PAGE_6(pppMath_FFT, ppMath, NamePage::MathFFT,
    "������", "SPECTRUM",
    "����������� ������� �������� �������",
    "Mapping the input signal spectrum",
    cMath_FFT_Enable,       // ������ - ���������� - ������ - �����������
    cMath_FFT_Scale,        // ������ - ���������� - ������ - �����
    cMath_FFT_Source,       // ������ - ���������� - ������ - ��������
    cMath_FFT_Window,       // ������ - ���������� - ������ - ����
    ppppMath_FFT_Cursors,   // ������ - ���������� - ������ - �������
    cMath_FFT_Limit,        // ������ - ���������� - ������ - ��������
    IsActive_Math_FFT, OnPress_Math_FFT, nullptr, nullptr
)

static bool IsActive_Math_FFT(void)
{
    return DISABLED_DRAW_MATH;
}

static void OnPress_Math_FFT(void)
{
    if (!IsActive_Math_FFT())
    {
        Display::ShowWarningBad(Warning::ImpossibleEnableFFT);
    }
}

// ������ - ���������� - ������ - ����������� --------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cMath_FFT_Enable, pppMath_FFT,
    "�����������", "Display",
    "�������� � ��������� ����������� �������",
    "Enables or disables the display of the spectrum",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    ENABLED_FFT, nullptr, nullptr, nullptr
)

// ������ - ���������� - ������ - ����� --------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cMath_FFT_Scale, pppMath_FFT,
    "�����", "Scale",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "��������", "Log",
    "��������", "Linear",
    SCALE_FFT, nullptr, nullptr, nullptr
)

// ������ - ���������� - ������ - �������� -----------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(cMath_FFT_Source, pppMath_FFT,
    "��������", "Source",
    "����� ��������� ��� ������� �������",
    "Selecting the source for the calculation of the spectrum",
    "����� 1",     "Channel 1",
    "����� 2",     "Channel 2",
    "����� 1 + 2", "Channel 1 + 2",
    SOURCE_FFT, nullptr, nullptr, nullptr
)

// ������ - ���������� - ������ - ���� ---------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(cMath_FFT_Window, pppMath_FFT,
    "����", "Window",
    "����� ���� ��� ������� �������",
    "Sets the window to calculate the spectrum",
    "�����������", "Rectangle",
    "��������",    "Hamming",
    "��������",    "Blackman",
    "�����",       "Hann",
    WINDOW_FFT, nullptr, nullptr, nullptr
)

// ������ - ���������� - ������ - ������� ------------------------------------------------------------------------------------------------------------
DEF_PAGE_6(ppppMath_FFT_Cursors, pppMath_FFT, NamePage::SB_MathCursorsFFT,
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

void *PageService::Math::FFT::Cursors::GetPointer()
{
    return (void *)&ppppMath_FFT_Cursors;
}

static bool IsActive_Math_FFT_Cursors(void)
{
    return ENABLED_FFT;
}

static void OnRegSet_Math_FFT_Cursors(int angle)
{
    FFT_POS_CURSOR(FFT_CUR_CURSOR) += (uint8)angle;
    Sound::RegulatorShiftRotate();
}

// ������ - ���������� - ������ - ������� - ����� ----------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(cMath_FFT_Cursors_Exit, ppppMath_FFT_Cursors,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, OnPress_Math_FFT_Cursors_Exit, DrawSB_Exit, nullptr
)

static void OnPress_Math_FFT_Cursors_Exit(void)
{
    Display::RemoveAddDrawFunction();
}

// ������ - ���������� - ������ - ������� - �������� -------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(cMath_FFT_Cursors_Source, ppppMath_FFT_Cursors,
    "��������", "Source",
    "����� ��������� ��� ������� �������",
    "Source choice for calculation of a range",
    nullptr, OnPress_Math_FFT_Cursors_Source, Draw_Math_FFT_Cursors_Source, nullptr
)

static void OnPress_Math_FFT_Cursors_Source(void)
{
    FFT_CUR_CURSOR = (uint8)((FFT_CUR_CURSOR + 1) % 2);
}

static void Draw_Math_FFT_Cursors_Source(int x, int y)
{
    Text(FFT_CUR_CURSOR_IS_0 ? "1" : "2").Draw(x + 7, y + 5);
}

// ������ - ���������� - ������ - �������� -----------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(cMath_FFT_Limit, pppMath_FFT,
    "��������", "Range",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "Here you can set the limit of monitoring the power spectrum",
    "-40��", "-40dB",
    "-60��", "-60dB",
    "-80��", "-80dB",
    FFT_MAX_DB, IsActive_Math_FFT_Limit, nullptr, nullptr
)

static bool IsActive_Math_FFT_Limit(void)
{
    return SCALE_FFT_IS_LOG;
}

// ������ - ETHERNET ///////////////
DEF_PAGE_5(ppEthernet, pService, NamePage::ServiceEthernet,
    "ETHERNET", "ETHERNET",
    "��������� ethernet",
    "Settings of ethernet",
    cEthernet_Enable,       // ������ - ETHERNET - Ethernet
    ipEthernet_IP,          // ������ - ETHERNET - IP �����
    ipEthernet_Mask,        // ������ - ETHERNET - ����� �������
    ipEthernet_Gateway,     // ������ - ETHERNET - ����
    macEthernet_MAC,        // ������ - ETHERNET - ��� �����
    nullptr, nullptr, nullptr, nullptr
)

// ������ - ETHERNET - Ethernet ----------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cEthernet_Enable, ppEthernet,
    "Ethernet", "Ethernet"
    ,
    "����� ������������� ethernet, �������� \"��������\" � ��������� ������.\n"
    "����� ��������� ethernet, �������� \"���������\" � ��������� ������."
    ,
    "To involve ethernet, choose \"Included\" and switch off the device.\n"
    "To disconnect ethernet, choose \"Disconnected\" and switch off the device."
    ,
    "��������",  "Included",
    "���������", "Disconnected",
    ETH_ENABLE, nullptr,  OnChanged_Ethernet_Enable, nullptr
)

static void OnChanged_Ethernet_Enable(bool)
{
    Display::ShowWarningGood(Warning::NeedRebootDevice2);
    Display::ShowWarningGood(Warning::NeedRebootDevice1);
}

// ������ - ETHERNET - IP ����� ----------------------------------------------------------------------------------------------------------------------
DEF_IPADDRESS(ipEthernet_IP, ppEthernet,
    "IP �����", "IP-address",
    "��������� IP ������",
    "Set of IP-address",
    IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3, PORT_ETH, OnChanged_Ethernet_Enable
)

// ������ - ETHERNET - ����� ������� -----------------------------------------------------------------------------------------------------------------
static uint16 portMask = 0;
DEF_IPADDRESS(ipEthernet_Mask, ppEthernet,
    "����� �������", "Network mask",
    "��������� ����� �������",
    "Set of network mask",
    NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3, portMask, OnChanged_Ethernet_Enable
)

// ������ - ETHERNET - ���� --------------------------------------------------------------------------------------------------------------------------
static uint16 portGateway = 0;
DEF_IPADDRESS(ipEthernet_Gateway, ppEthernet,
    "����", "Gateway",
    "��������� ������ ��������� �����",
    "Set of gateway address",
    GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3, portGateway, OnChanged_Ethernet_Enable
)

// ������ - ETHERNET - ��� ����� ---------------------------------------------------------------------------------------------------------------------
DEF_MACADDRESS(macEthernet_MAC, ppEthernet,
    "��� �����", "MAC-address",
    "��������� ����������� ������",
    "Set of MAC-address",
    MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5, OnChanged_Ethernet_Enable
)

// ������ - ���� -------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cSound, pService,
    "����", "Sound",
    "���������/���������� �����",
    "Inclusion/switching off of a sound",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SOUND_ENABLED, nullptr, nullptr, nullptr
)


// ������ - ���� -------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cLang, pService,
    "����", "Language",
    "��������� ������� ���� ����",
    "Allows you to select the menu language",
    "�������",    "Russian",
    "����������", "English",
    LANG, nullptr, nullptr, nullptr
)


// ������ - ����� ------------------------------------------------------------------------------------------------------------------------------------
static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
DEF_TIME(tTime, pService,
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

// ������ - ��� ���� ����� ---------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cModeLongPressButtonTrig, pService,
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
    MODE_LONG_PRESS_TRIG, nullptr, nullptr, nullptr
)

// ������ - ���������� -------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_6(ppInformation, pService, NamePage::SB_Information,
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

const Page *PageService::Information::self = &ppInformation;


static void OnPress_Information(void)
{
    PageService::Information::self->OpenAndSetItCurrent();
    Display::SetDrawMode(DrawMode::Hand, Information_Draw);
}

static void Information_Draw(void)
{
    Painter::BeginScene(Color::BLACK);
    int x = 100;
    int dY = 20;
    int y = 20;
    Rectangle(319, 239).Draw(0, 0, COLOR_FILL);
    y += dY;
    Text(LANG_RU ? "����������" : "INFORMATION").Draw(x, y);
    y += dY;
    Text(LANG_RU ? "������ : �8-53/1" : "Model : S8-53/1").Draw(x, y);
    y += dY;

    char buffer[100];
    /*
    OTP_GetSerialNumber(buffer);
    if (buffer[0])
    {
        Painter::DrawFormatText(x, y, COLOR_FILL, lang == Russian ? "C/� : %s" : "S/N : %s", buffer);
        y += dY;
    }
    */

    Text(LANG_RU ? "����������� �����������:" : "Software:").Draw(x, y);
    y += dY;
    std::sprintf(buffer, (const char*)(LANG_RU ? "������ %s" : "version %s"), NUM_VER);
    Text(buffer).Draw(x, y);
    y += dY;

    Painter::DrawFormatText(x, y, COLOR_FILL, "CRC32 : %X", HAL_CRC::Calculate());

    dY = -10;
    Painter::DrawStringInCenterRect(0, 190 + dY, 320, 20, "��� ��������� ������ ������� � ����������� ������ ������");
    Painter::DrawStringInCenterRect(0, 205 + dY, 320, 20, "����� ����������: ���./����. 8-017-270-02-00");
    Painter::DrawStringInCenterRect(0, 220 + dY, 320, 20, "������������: e-mail: mnipi-24(@)tut.by, ���. 8-017-270-02-23");

    Menu::Draw();
    Painter::EndScene();
}

// ������ - ���������� - ����� -----------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(sbInformation_Exit, ppInformation,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, OnPress_Information_Exit, DrawSB_Exit, nullptr
)

static void OnPress_Information_Exit(void)
{
    Display::SetDrawMode(DrawMode::Auto, 0);
    Display::RemoveAddDrawFunction();
}
