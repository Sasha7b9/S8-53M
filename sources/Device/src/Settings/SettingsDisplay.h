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
#define LAST_AFFECTED_CHANNEL_IS_A  (LAST_AFFECTED_CHANNEL == Channel::A)       // \c true, ���� ��������� �����, ������� ��������� - 1.
#define LAST_AFFECTED_CHANNEL_IS_B  (LAST_AFFECTED_CHANNEL == Channel::B)       // \c true, ���� ��������� �����, ������� ��������� - 2.

#define MODE_ACCUM                  (set.display.modeAccumulation)              // SettingsDisplay.modeAccumulation
#define MODE_ACCUM_IS_RESET         (MODE_ACCUM == ModeAccumulation::Reset)     // \c true, ���� ���������� �� �������.
#define MODE_ACCUM_IS_NORESET       (MODE_ACCUM == ModeAccumulation::NoReset)   // \c true, ���� ���������� ��� ������.

#define ALT_MARKERS                 (set.display.altMarkers)                    // SettingsDisplay.altMarkers
#define ALT_MARKERS_HIDE            (ALT_MARKERS == AltMarkers::Hide)           // \c true, ���� �������������� ������� ������� �� ���� ����������.

#define MENU_AUTO_HIDE              (set.display.menuAutoHide)                  // SettingsDisplay.menuAutoHide

#define SHOW_STRING_NAVIGATION      (set.display.showStringNavigation)          // SettingsDisplay.showStringNavigation

#define LINKING_RSHIFT              (set.display.linkingRShift)                 // SettingsDisplay.linkingRShift
#define LINKING_RSHIFT_IS_VOLTAGE   (LINKING_RSHIFT == LinkingRShift::Voltage)   // \c true, ���� �������� � ����������� ����������.


// ����� ����������.
struct ModeAccumulation { enum E
{
    NoReset,   // � ���� ������ ������������ ������ N ��������� ���������.
    Reset      // � ���� ������ ���������� N ��������� ��������� � ����� ������������.
}; };

// ����� ����������� �������������� ������� �������� �������� �� ����������.
struct AltMarkers { enum E
{
    Hide,        // ������ �� ��������.
    Show,        // ������ ��������.
    AutoHide     // �������� � ������� ����� timeShowLevels.
}; };

// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
struct MenuAutoHide { enum E
{
    None,   // �������.
    _5,     // ����� 5 ������.
    _10,    // ����� 10 ������.
    _15,    // ����� 15 ������.
    _30,    // ����� 30 ������.
    _60     // ����� 60 ������.
    };

    // ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������.
    static int Time();
 };

// ��� �������� � �������� �� ���������
struct LinkingRShift { enum E
{
    Voltage,      // �������� ��������� � ����������
    Position      // �������� ���������� � ������� �� ������
}; };



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
    ENumAveraging::E    enumAve;                    // ������������ ���������� �������.
    ModeAveraging::E    modeAve;                    // ��� ���������� �� ����������.
    ENumMinMax::E       enumMinMax;                 // ����� ��������� ��� ����������� ��������� � ����������.
    Smoothing::E        smoothing;                  // ����� ����� ��� ����������� �������.
    ENumSignalsInSec::E enumSignalsInSec;           // ������������ ���������� ������� � �������.
    Channel::E          lastAffectedChannel;        // ����� �������� ����� ���������� ������, ������� ��������� �������. ����� ��� ����, ����� �����, ����� ������ �������� �������.
    ModeAccumulation::E modeAccumulation;           // ����� ����� ���������� ��������.
    AltMarkers::E       altMarkers;                 // ����� ����������� �������������� ������� �������� ��������.
    MenuAutoHide::E     menuAutoHide;               // ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    bool                showFullMemoryWindow;       // ���������� �� ���� ������ ������ ������. \todo �� ������������.
    bool                showStringNavigation;       // ���������� �� ������ �������� ��������� ����..
    LinkingRShift::E    linkingRShift;              // ��� �������� �������� �� ���������.

    // ���������� ����� ������ � ��������� ����� �� ������.
    static void PointsOnDisplay(int *firstPoint, int *lastPoint);

    // ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
    static bool IsSeparate();
};


/** @}  @}
 */
