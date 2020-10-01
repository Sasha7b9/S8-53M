#pragma once
#include "defines.h"
#include "SettingsTypes.h"
#include "Display/Colors.h"


/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsDisplay
 *  @{
 */



#define TIME_SHOW_LEVELS            (set.display.timeShowLevels)                // SettingsDisplay.timeShowLevels

#define SHIFT_IN_MEMORY             (set.display.shiftInMemory)                 // SettingsDisplay.shiftInMemory

#define TIME_MESSAGES               (set.display.timeMessages)                  // SettingsDisplay.timeMessages

#define BRIGHTNESS                  (set.display.brightness)                    // SettingsDisplay.brightness

#define BRIGHTNESS_GRID             (set.display.brightnessGrid)                // SettingsDisplay.brightnessGrid

#define DISPLAY_COLOR(num)          (set.display.colors[num])                   // SettingsDisplay.colors

#define MODE_DRAW_SIGNAL            (set.display.modeDrawSignal)                // SettingsDisplay.modeDrawSignal
#define MODE_DRAW_IS_SIGNAL_LINES   (MODE_DRAW_SIGNAL == ModeDrawSignal::Lines)  // \c true, ���� ������ ��������� �������.

#define TYPE_GRID                   (set.display.typeGrid)                      // SettingsDisplay.typeGrid
#define TYPE_GRID_IS_1              (TYPE_GRID == TypeGrid::_1)                 // ���������� ��� ����� TypeGrid::_1.
#define TYPE_GRID_IS_2              (TYPE_GRID == TypeGrid::_2)                 // ���������� ��� ����� TypeGrid_2.
#define TYPE_GRID_IS_3              (TYPE_GRID == TypeGrid::_3)                 // ���������� ��� ����� TypeGrid_3.

#define ENUM_ACCUM                  (set.display.enumAccumulation)              // SettingsDisplay.numAccumulation
#define ENUM_ACCUM_IS_NONE          (ENUM_ACCUM == ENumAccumulation::_1)        // \c true, ���� ���������� ���������.
#define ENUM_ACCUM_IS_INFINITY      (ENUM_ACCUM == ENumAccumulation::Infinity)  // \c true, ���� ����������� ����������� ����� ����������.
#define NUM_ACCUM                   (1 << (int)set.display.enumAccumulation)    // ���������� ����������.

#define ENUM_AVE                    (set.display.enumAve)                       // SettingsDisplay.enumAve
#define NUM_AVE                     (1 << ENUM_AVE)                             // ���������� ����������.

#define MODE_AVE                    (set.display.modeAve)                       // SettingsDisplay.modeAve

#define ENUM_MIN_MAX                (set.display.enumMinMax)                    // SettingsDisplay.enumMinMax
#define NUM_MIN_MAX                 (1 << ENUM_MIN_MAX)                         // ���������� ��������� �������� ��� ������� ��������� � ����������.

#define SMOOTHING                   (set.display.smoothing)                     // SettingsDisplay.smoothing
#define SMOOTHING_IS_DISABLE        (SMOOTHING == Smoothing::Disable)            // \c true, ���� ���������� ������ ��������.

#define ENUM_SIGNALS_IN_SEC         (set.display.enumSignalsInSec)              // SettingsDisplay.enumSignalsInSec

#define LAST_AFFECTED_CHANNEL       (set.display.lastAffectedChannel)           // SettingsDisplay.lastAffectedChannel
#define LAST_AFFECTED_CHANNEL_IS_A  (LAST_AFFECTED_CHANNEL == A)                // \c true, ���� ��������� �����, ������� ��������� - 1.
#define LAST_AFFECTED_CHANNEL_IS_B  (LAST_AFFECTED_CHANNEL == B)                // \c true, ���� ��������� �����, ������� ��������� - 2.

#define MODE_ACCUM                  (set.display.modeAccumulation)              // SettingsDisplay.modeAccumulation
#define MODE_ACCUM_IS_RESET         (MODE_ACCUM == ModeAccumulation_Reset)      // \c true, ���� ���������� �� �������.
#define MODE_ACCUM_IS_NORESET       (MODE_ACCUM == ModeAccumulation_NoReset)    // \c true, ���� ���������� ��� ������.

#define ALT_MARKERS                 (set.display.altMarkers)                    // SettingsDisplay.altMarkers
#define ALT_MARKERS_HIDE            (ALT_MARKERS == AM_Hide)                    // \c true, ���� �������������� ������� ������� �� ���� ����������.

#define MENU_AUTO_HIDE              (set.display.menuAutoHide)                  // SettingsDisplay.menuAutoHide

#define SHOW_STRING_NAVIGATION      (set.display.showStringNavigation)          // SettingsDisplay.showStringNavigation

#define LINKING_RSHIFT              (set.display.linkingRShift)                 // SettingsDisplay.linkingRShift
#define LINKING_RSHIFT_IS_VOLTAGE   (LINKING_RSHIFT == LinkingRShift_Voltage)   // \c true, ���� �������� � ����������� ����������.



 // ����� ��������� �������.
struct ModeDrawSignal { enum E
{
    Lines,      // ������ �������� �������.
    Points      // ������ �������� �������.
}; };

// ��� ����� �� ������.
struct TypeGrid { enum E
{
    _1,
    _2,
    _3,
    _4,
    Count
}; };

// ������������ ����������.
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
}; };

// ���������� ���������� �� ����������.
enum ENumAveraging
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

// ��� ���������� �� ����������.
enum ModeAveraging
{
    Averaging_Accurately,   // ��������� �����.
    Averaging_Around        // ��������� ��������������.
};

// ���������� ��������� ��� ������� ������������ � ������������� ��������.
enum ENumMinMax
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

// ���������� ����� ��� ������� ����������� �������.
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
}; };

// ����������� FPS.
enum ENumSignalsInSec
{
    NumSignalsInSec_25,
    NumSignalsInSec_10,
    NumSignalsInSec_5,
    NumSignalsInSec_2,
    NumSignalsInSec_1
};

// ����� ����������.
enum ModeAccumulation
{
    ModeAccumulation_NoReset,   // � ���� ������ ������������ ������ N ��������� ���������.
    ModeAccumulation_Reset      // � ���� ������ ���������� N ��������� ��������� � ����� ������������.
};

// ����� ����������� �������������� ������� �������� �������� �� ����������.
enum AltMarkers
{
    AM_Hide,        // ������ �� ��������.
    AM_Show,        // ������ ��������.
    AM_AutoHide     // �������� � ������� ����� timeShowLevels.
};

// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
enum MenuAutoHide
{
    MenuAutoHide_None = 0,  // �������.
    MenuAutoHide_5 = 5,     // ����� 5 ������.
    MenuAutoHide_10 = 10,   // ����� 10 ������.
    MenuAutoHide_15 = 15,   // ����� 15 ������.
    MenuAutoHide_30 = 30,   // ����� 30 ������.
    MenuAutoHide_60 = 60    // ����� 60 ������.
};

// ��� �������� � �������� �� ���������
enum LinkingRShift
{
    LinkingRShift_Voltage,      // �������� ��������� � ����������
    LinkingRShift_Position      // �������� ���������� � ������� �� ������
};



// ��������� �����������
struct SettingsDisplay
{
    int16               timeShowLevels;             // �����, � ������� �������� ����� ���������� ������ ��������.
    int16               shiftInMemory;              // ���������� �������� ������ ���� ����� ������������ �������� ����� ������. ����� ��� ����������� ����������� ������� � ����.
    int16               timeMessages;               // ����� � ��������, �� ������� ��������� ������� �� ������.
    int16               brightness;                 // ������� �������.
    int16               brightnessGrid;             // ������� ����� �� 0 �� 100.
    uint16              colors[16];                 // �����.
    ModeDrawSignal::E   modeDrawSignal;             // ����� ��������� �������.
    TypeGrid::E         typeGrid;                   // ��� �����.
    ENumAccumulation::E enumAccumulation;           // ������������ ���������� ������� �� ������.
    ENumAveraging       enumAve;                    // ������������ ���������� �������.
    ModeAveraging       modeAve;                    // ��� ���������� �� ����������.
    ENumMinMax          enumMinMax;                 // ����� ��������� ��� ����������� ��������� � ����������.
    Smoothing::E        smoothing;                  // ����� ����� ��� ����������� �������.
    ENumSignalsInSec    enumSignalsInSec;           // ������������ ���������� ������� � �������.
    Channel             lastAffectedChannel;        // ����� �������� ����� ���������� ������, ������� ��������� �������. ����� ��� ����, ����� �����, ����� ������ �������� �������.
    ModeAccumulation    modeAccumulation;           // ����� ����� ���������� ��������.
    AltMarkers          altMarkers;                 // ����� ����������� �������������� ������� �������� ��������.
    MenuAutoHide        menuAutoHide;               // ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    bool                showFullMemoryWindow;       // ���������� �� ���� ������ ������ ������. \todo �� ������������.
    bool                showStringNavigation;       // ���������� �� ������ �������� ��������� ����..
    LinkingRShift       linkingRShift;              // ��� �������� �������� �� ���������.
};



// ���������� ����������� ������� ������.
int sDisplay_NumSignalsInS();
// ������������� ����������� ������� ������.
void sDisplay_SetNumSignalsInS(int maxFPS);
// ���������� ����� ����� ������������� ������� (����� ������� - �����������).
int sDisplay_NumPointSmoothing();
// ���������� ����� ������ � ��������� ����� �� ������.
void sDisplay_PointsOnDisplay(int *firstPoint, int *lastPoint);
// ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������.
int sDisplay_TimeMenuAutoHide();
// ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
bool sDisplay_IsSeparate();
// brightness = 1..100
void sDisplay_SetBrightness(int16 brightness);
// ���������� ����� ����������
ModeAveraging sDisplay_ModeAveraging();

int sDisplay_NumAverage();


/** @}  @}
 */
