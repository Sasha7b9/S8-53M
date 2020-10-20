#include "defines.h"
#include "Menu/MenuItems.h"
#include "defines.h"
#include "Menu/Menu.h"
#include "Tables.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


extern const Page pDisplay;


extern const Choice mcMapping;                              //     ������� - �����������

extern const Page mspAccumulation;                          //     ������� - ����������
static bool IsActive_Accumulation();                        // ������� �� �������� �������-����������
extern const Choice mcAccumulation_Number;                  // ������� - ���������� - ����������
extern const Choice mcAccumulation_Mode;                    // ������� - ���������� - �����
extern const Button mbAccumulation_Clear;                   // ������� - ���������� - ��������
static bool IsActive_Accumulation_Clear();                  // ������� �� ������ �������-����������-��������
void OnPress_Accumulation_Clear();                          // ��������� ������� �������-����������-��������

extern const Page mspAveraging;                             //     ������� - ����������
static bool IsActive_Averaging();                           // ������� �� �������� �������-����������
extern const Choice mcAveraging_Number;                     // ������� - ���������� - ����������
extern const Choice mcAveraging_Mode;                       // ������� - ���������� - �����

extern const Choice mcMinMax;                               //     ������� - ��� ����
static bool IsActive_MinMax();                              // ������� �� ��������� �������-��� ����
static void OnChanged_MinMax(bool active);                  // ������� �� ��������� �������-��� ����

extern const Choice mcSmoothing;                            //     ������� - �����������

extern const Choice mcRefreshFPS;                           //     ������� - ������� ������
void OnChanged_RefreshFPS(bool active);                     // ������� �� ��������� �������-������� ������
extern const Page mspGrid;                                  //     ������� - �����

extern const Choice mcGrid_Type;                            //     ������� - ����� - ���
extern const Governor mgGrid_Brightness;                    // ������� - ����� - �������
void OnChanged_Grid_Brightness();                           // ������� �� ��������� �������-�����-�������
static void BeforeDraw_Grid_Brightness();                   // ���������� ����� ���������� �������-�����-�������

extern const Choice mcTypeShift;                            //     ������� - ��������

extern const Page mspSettings;                              //     ������� - ���������
extern const Page mspSettings_Colors;                       // ������� - ��������� - �����
extern const Choice mcSettings_Colors_Scheme;               // ������� - ��������� - ����� - �������� �����
extern const GovernorColor mgcSettings_Colors_ChannelA;     // ������� - ��������� - ����� - ����� 1
extern const GovernorColor mgcSettings_Colors_ChannelB;     // ������� - ��������� - ����� - ����� 2
extern const GovernorColor mgcSettings_Colors_Grid;         // ������� - ��������� - ����� - �����
extern const Governor mgSettings_Brightness;                // ������� - ��������� - �������
static void OnChanged_Settings_Brightness();                // ���������� ��� ��������� �������-���������-�������
extern const Governor mgSettings_Levels;                    // ������� - ��������� - ������
extern const Governor mgSettings_TimeMessages;              // ������� - ��������� - �����
extern const Choice mcSettings_ShowStringNavigation;        // ������� - ��������� - ������ ����
extern const Choice mcSettings_ShowAltMarkers;              // ������� - ��������� - ���. �������
static void OnChanged_Settings_ShowAltMarkers(bool);
extern const Choice mcSettings_AutoHide;                    // ������� - ��������� - ��������
static void OnChanged_Settings_AutoHide(bool autoHide);     // ���������� ��� ��������� �������-���������-��������


// ������� /////////////////////////
extern const Page mainPage;

DEF_PAGE_9(pDisplay, mainPage, NamePage::Display,
    "�������", "DISPLAY",
    "�������� ��������� ����������� �������.",
    "Contains settings of display of the Display::",
    mcMapping,
    mspAccumulation,
    mspAveraging,
    mcMinMax,
    mcSmoothing,
    mcRefreshFPS,
    mspGrid,
    mcTypeShift,
    mspSettings,
    nullptr, nullptr, nullptr, nullptr
)


// ������� - ����������� -----------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcMapping, pDisplay,
    "�����������", "View",
    "����� ����� ����������� �������.",
    "Sets the display mode signal.",
    "������", "Vector",
    "�����",  "Points",
    MODE_DRAW_SIGNAL, nullptr, nullptr, nullptr
)


// ������� - ���������� ////////////
DEF_PAGE_3(mspAccumulation, pDisplay, NamePage::DisplayAccumulation,
    "����������", "ACCUMULATION",
    "��������� ������ ����������� ��������� �������� �� ������.",
    "Mode setting signals to display the last screen.",
    mcAccumulation_Number,
    mcAccumulation_Mode,
    mbAccumulation_Clear,
    IsActive_Accumulation, nullptr, nullptr, nullptr
)

static bool IsActive_Accumulation(void)
{
    return SET_TBASE > TBase::_50ns;
}

// ������� - ���������� - ���������� -----------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_9(mcAccumulation_Number, mspAccumulation,
    "����������", "Number"
    ,
    "����� ������������ ���������� ��������� �������� �� ������. ���� � ��������� \"�����\" ������� \"�������������\", ����� ��������� ������ "
    "�������� ������ \"��������\"."
    "\"�������������\" - ������ ��������� ������� �� ������� �� ��� ���, ���� �� ����� ������ ������ \"��������\"."
    ,
    "Sets the maximum quantity of the last signals on the screen. If in control \"Mode\" it is chosen \"Infinity\", the screen is cleared only "
    "by pressing of the button \"Clear\"."
    "\"Infinity\" - each measurement remains on the display until the button \"Clear\" is pressed."
    ,
    DISABLE_RU,      DISABLE_EN,
    "2",             "2",
    "4",             "4",
    "8",             "8",
    "16",            "16",
    "32",            "32",
    "64",            "64",
    "128",           "128",
    "�������������", "Infinity",
    ENUM_ACCUM, nullptr, nullptr, nullptr
)


// ������� - ���������� - ����� ----------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcAccumulation_Mode, mspAccumulation,
    "�����", "Mode"
    ,
    "1. \"����������\" - ����� ���������� ��������� ���������� ��������� ���������� ������� �������. ���� ����� ������, ����� ������ �� ������� "
    "��� ���������� ������� ���������� ���������.\n"
    "2. \"�� ����������\" - �� ������� ������ ��������� �������� ��� ������� (� ������ �������� ������) ���������� ���������. ����������� �������� "
    "������� �������������� � ������������� ���������� ��������� ���������� ��������� ��� ���������� ������."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the Display:: This mode is convenient when memory "
    "isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of shortage of memory). Shortcoming "
    "is smaller speed and impossibility of accumulation of the set number of measurements at a lack of memory."
    ,
    "�� ����������", "Not to dump",
    "����������",    "Dump",
    MODE_ACCUM, nullptr, nullptr, nullptr
)


// ������� - ���������� - �������� /
DEF_BUTTON(mbAccumulation_Clear, mspAccumulation,
    "��������", "Clear",
    "������� ����� �� ����������� ��������.",
    "Clears the screen of the saved-up signals.",
    IsActive_Accumulation_Clear, OnPress_Accumulation_Clear
)


static bool IsActive_Accumulation_Clear(void)
{
    return ENUM_ACCUM_IS_INFINITY;
}

void OnPress_Accumulation_Clear(void)
{
    Display::Redraw();
}


// ������� - ���������� ////////////
DEF_PAGE_2(mspAveraging, pDisplay, NamePage::DisplayAverage,
    "����������", "AVERAGE",
    "��������� ������ ���������� �� ��������� ����������.",
    "Settings of the mode of averaging on the last measurements.",
    mcAveraging_Number,
    mcAveraging_Mode,
    IsActive_Averaging, nullptr, nullptr, nullptr
)

static bool IsActive_Averaging(void)
{
    return true;
}


// ������� - ���������� - ���������� -----------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(mcAveraging_Number, mspAveraging,
    "����������", "Number",
    "����� ���������� ��������� ���������, �� ������� ������������ ����������.",
    "Sets number of the last measurements on which averaging is made.",
    DISABLE_RU,    DISABLE_EN,
    "2",           "2",
    "4",           "4",
    "8",           "8",
    "16",          "16",
    "32",          "32",
    "64",          "64",
    "128",         "128",
    "256",         "256",
    "512",         "512",
    ENUM_AVE, nullptr, nullptr, nullptr
)

// ������� - ���������� - ����� ----------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcAveraging_Mode, mspAveraging,
    "�����", "Mode"
    ,
    "1. \"�����\" - ������ ����� ����������, ����� � ������� ��������� ������ ��������� �������.\n"
    "2. \"��������������\" - ��������������� ����� ����������. ����� ����� ������������, ����� ������ ���������� ��������� �������, ��� ����� "
    "����������� � ������."
    ,
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory.",
    "�����",          "Accurately",
    "��������������", "Around",
    MODE_AVE, nullptr, nullptr, nullptr
)


// ������� - ��� ���� --------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_8(mcMinMax, pDisplay,
    "��� ����", "Min Max",
    "����� ���������� ��������� ���������, �� ������� �������� ��������������� �����, ��������� �������� � ��������� ���������.",
    "Sets number of the last measurements on which the limiting lines which are bending around minima and maxima of measurements are under construction.",
    DISABLE_RU, DISABLE_EN,
    "2",        "2",
    "4",        "4",
    "8",        "8",
    "16",       "16",
    "32",       "32",
    "64",       "64",
    "128",      "128",
    ENUM_MIN_MAX, IsActive_MinMax, OnChanged_MinMax, nullptr
)

static bool IsActive_MinMax(void) //-V524
{
    return SET_TBASE > TBase::_50ns;
}

static void OnChanged_MinMax(bool)
{
    /*
    int maxMeasures = DS_NumberAvailableEntries();  
    int numMinMax = sDisplay_NumberMinMax();

    if (maxMeasures < numMinMax)
    {
        Display::ShowWarningWithNumber(ExcessValues, maxMeasures);
    }
    */
}



// ������� - ����������� -----------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(mcSmoothing, pDisplay,
    "�����������", "Smoothing",
    "������������� ���������� ����� ��� ������� ����������� �� �������� ������ �������.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points.",
    DISABLE_RU, DISABLE_EN,
    "2 �����",  "2 points",
    "3 �����",  "3 points",
    "4 �����",  "4 points",
    "5 �����",  "5 points",
    "6 �����",  "6 points",
    "7 �����",  "7 points",
    "8 �����",  "8 points",
    "9 �����",  "9 points",
    "10 �����", "10 points",
    SMOOTHING, nullptr, nullptr, nullptr
)


// ������� - ������� ������ --------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_5(mcRefreshFPS, pDisplay,
    "������� ������", "Refresh rate",
    "����� ������������ ����� ��������� � ������� ������.",
    "Sets the maximum number of the shots removed in a second.",
    "25", "25",
    "10", "10",
    "5",  "5",
    "2",  "2",
    "1",  "1",
    ENUM_SIGNALS_IN_SEC, nullptr, OnChanged_RefreshFPS, nullptr
)

void OnChanged_RefreshFPS(bool)
{
    FPGA::SetNumSignalsInSec(ENumSignalsInSec::NumSignalsInS());
}


// ������� - ����� /////////////////
DEF_PAGE_2(mspGrid, pDisplay, NamePage::DisplayGrid,
    "�����", "GRID",
    "�������� ��������� ����������� ������������ �����.",
    "Contains settings of display of a coordinate Grid::",
    mcGrid_Type,
    mgGrid_Brightness,
    nullptr, nullptr, nullptr, nullptr
)


// ������� - ����� - ��� -----------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(mcGrid_Type, mspGrid,
    "���", "Type",
    "����� ���� �����.",
    "Choice like Grid.",
    "��� 1", "Type 1",
    "��� 2", "Type 2",
    "��� 3", "Type 3",
    "��� 4", "Type 4",
    TYPE_GRID, nullptr, nullptr, nullptr
)


// ������� - ����� - ������� -------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgGrid_Brightness, mspGrid,
    "�������", "Brightness",
    "������������� ������� �����.",
    "Adjust the brightness of the Grid.",
    BRIGHTNESS_GRID, 0, 100, nullptr, OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
)


ColorType colorTypeGrid = {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID};

void OnChanged_Grid_Brightness(void)
{
    Color_SetBrightness(&colorTypeGrid, BRIGHTNESS_GRID / 1e2F);
}

static void BeforeDraw_Grid_Brightness(void)
{
    Color_Init(&colorTypeGrid);
    BRIGHTNESS_GRID = (int16)(colorTypeGrid.brightness * 100.0F);
}


// ������� - �������� --------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcTypeShift, pDisplay,
    "��������", "Offset",
    "����� ����� ��������� �������� �� ���������\n1. \"����������\" - ����������� ���������� ��������.\n2. \"�������\" - ����������� ��������� �������� �� ������.",
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of the offset on the screen.",
    "����������", "Voltage",
    "�������",    "Divisions",
    LINKING_RSHIFT, nullptr, nullptr, nullptr
)


// ������� - ��������� /////////////
DEF_PAGE_7(mspSettings, pDisplay, NamePage::ServiceDisplay,
    "���������", "SETTINGS",
    "�������������� ��������� �������",
    "Additional display settings",
    mspSettings_Colors,
    mgSettings_Brightness,
    mgSettings_Levels,
    mgSettings_TimeMessages,
    mcSettings_ShowStringNavigation,
    mcSettings_ShowAltMarkers,
    mcSettings_AutoHide,
    nullptr, nullptr, nullptr, nullptr
)


// ������� - ��������� - ����� /////
DEF_PAGE_4(mspSettings_Colors, mspSettings, NamePage::ServiceDisplayColors,
    "�����", "COLORS",
    "����� ������ �������",
    "The choice of colors display",
    mcSettings_Colors_Scheme,
    mgcSettings_Colors_ChannelA,
    mgcSettings_Colors_ChannelB,
    mgcSettings_Colors_Grid,
    nullptr, nullptr, nullptr, nullptr
)


// ������� - ��������� - ����� - �������� ����� ------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcSettings_Colors_Scheme, mspSettings_Colors,
    "�������� �����", "Color scheme",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "����� 1", "Scheme 1",
    "����� 2", "Scheme 2",
    COLOR_SCHEME, nullptr, nullptr, nullptr
)

static ColorType colorT1 = {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_A};

// ������� - ��������� - ����� - ����� 1 -------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelA, mspSettings_Colors,
    "����� 1", "Channel 1",
    "",
    "",
    colorT1, nullptr
)


static ColorType colorT2 = {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_B};

// ������� - ��������� - ����� - ����� 2 -------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelB, mspSettings_Colors,
    "����� 2", "Channel 2",
    "",
    "",
    colorT2, nullptr
)


// ������� - ��������� - ����� - ����� ---------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR_COLOR(mgcSettings_Colors_Grid, mspSettings_Colors,
    "�����", "Grid",
    "������������� ���� �����",
    "Sets the grid color",
    colorTypeGrid, nullptr
)


// ������� - ��������� - ������� ---------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgSettings_Brightness, mspSettings,
    "�������", "Brightness",
    "��������� ������� �������� �������",
    "Setting the brightness of the display",
    BRIGHTNESS, 0, 100, nullptr, OnChanged_Settings_Brightness, nullptr
)

static void OnChanged_Settings_Brightness(void)
{
    Painter::SetBrightnessDisplay(BRIGHTNESS);
}


// ������� - ��������� - ������ ----------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgSettings_Levels, mspSettings,
    "������", "Levels",
    "����� �����, � ������� �������� ����� �������� ����� ������� ���������� �� ������ ������� ��������������� ����� ������ ��������",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    TIME_SHOW_LEVELS, 0, 125, nullptr, nullptr, nullptr
)

// ������� - ��������� - ����� -----------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgSettings_TimeMessages, mspSettings,
    "�����", "Time",
    "��������� �������, � ������� �������� ��������� ����� ���������� �� ������",
    "Set the time during which the message will be on the screen",
    TIME_MESSAGES, 1, 99, nullptr, nullptr, nullptr
)


// ������� - ��������� - ������ ���� -----------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcSettings_ShowStringNavigation, mspSettings,
    "������ ����", "Path menu",
    "��� ������ \n����������\n ����� ������ ������ ��������� ������ ���� �� ������� �������� ����",
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "��������",   "Hide",
    "����������", "Show",
    SHOW_STRING_NAVIGATION, nullptr, nullptr, nullptr
)


// ������� - ��������� - ���. ������� ----------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcSettings_ShowAltMarkers, mspSettings,
    "���. �������", "Alt. markers"
    ,
    "������������� ����� ����������� �������������� �������� ������� �������� � �������������:\n"
    "\"��������\" - �������������� ������� �� ������������,\n"
    "\"����������\" - �������������� ������� ������������ ������,\n"
    "\"����\" - �������������� ������� ������������ � ������� 5 ��� ����� �������� ����� �������� ������ �� ���������� ��� ������ �������������"
    ,
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level"
    ,
    "��������",   "Hide",
    "����������", "Show",
    "����",       "Auto",
    ALT_MARKERS, nullptr, OnChanged_Settings_ShowAltMarkers, nullptr
)

static void OnChanged_Settings_ShowAltMarkers(bool)
{
    Display::ChangedRShiftMarkers();
}


// ������� - ��������� - �������� --------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_6(mcSettings_AutoHide, mspSettings,
    "��������", "Hide",
    "��������� ����� ���������� ������� ������ ��� �������� �����, �� ��������� �������� ���� ������������� ��������� � ������",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "�������",      "Never",
    "����� 5 ���",  "Through 5 s",
    "����� 10 ���", "Through 10 s",
    "����� 15 ���", "Through 15 s",
    "����� 30 ���", "Through 30 s",
    "����� 60 ���", "Through 60 s",
    MENU_AUTO_HIDE, nullptr, OnChanged_Settings_AutoHide, nullptr
)

static void OnChanged_Settings_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}


// ������� - ���� ������ ///////////
/*
static const Choice mcDisplMemoryWindow =
{
    TypeItem::Choice, &pDisplay, 0,
    {
        "���� ������", "Window memory",
        "1. \"�����������\" - � ������� ����� ������ ��������� ���������� ������.\n2. \"����������\" - ��������� ��������� �������� ���� � ������.",
        "1. \"Standard\" - in the top part of the screen memory contents are removed.\n2. \"Simplified\" - shows the position of the visible window in memory."
    },
    {
        {"����������",  "Simplified"},
        {"�����������", "Standard"}
    },
    (int8*)&set.display.showFullMemoryWindow
};
*/
