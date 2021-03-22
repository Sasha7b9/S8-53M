#pragma once
#include "common/Display/Colors_.h"


#define BRIGHTNESS_GRID             (set.display.brightnessGrid)                // SettingsDisplay.brightnessGrid

#define DISPLAY_COLOR(num)          (set.display.colors[num])                   // SettingsDisplay.colors

#define ENUM_AVE                    (set.display.enumAve)                       // SettingsDisplay.enumAve
#define NUM_AVE                     (1 << ENUM_AVE)                             // ���������� ����������.

#define MODE_AVE                    (set.display.modeAve)                       // SettingsDisplay.modeAve

#define ENUM_MIN_MAX                (::set.display.enumMinMax)                  // SettingsDisplay.enumMinMax
#define NUM_MIN_MAX                 (1 << ENUM_MIN_MAX)                         // ���������� ��������� �������� ��� ������� ��������� � ����������.

#define ENUM_SIGNALS_IN_SEC         (set.display.enumSignalsInSec)              // SettingsDisplay.enumSignalsInSec

#define MENU_AUTO_HIDE              (set.display.menuAutoHide)                  // SettingsDisplay.menuAutoHide

#define SHOW_STRING_NAVIGATION      (set.display.showStringNavigation)          // SettingsDisplay.showStringNavigation

// ��������� �����������
struct SettingsDisplay
{
    int16               time_show_levels;      // �����, � ������� �������� ����� ���������� ������ ��������.
    int16               shift_in_memory;       // ���������� �������� ������ ���� ����� ������������ �������� �����
                                               // ������. ����� ��� ����������� ����������� ������� � ����.
    int16               time_messages;         // ����� � ��������, �� ������� ��������� ������� �� ������.
    int16               brightness;            // ������� �������.
    int16               brightnessGrid;        // ������� ����� �� 0 �� 100.
    uint                colors[16];            // �����.
    ModeDrawSignal::E   mode_draw_signal;      // ����� ��������� �������.
    TypeGrid::E         typeGrid;              // ��� �����.
    ENumAccumulation::E enum_accum;            // ������������ ���������� ������� �� ������.
    ENumAveraging::E    enumAve;               // ������������ ���������� �������.
    ModeAveraging::E    modeAve;               // ��� ���������� �� ����������.
    ENumMinMax::E       enumMinMax;            // ����� ��������� ��� ����������� ��������� � ����������.
    Smoothing::E        smoothing;             // ����� ����� ��� ����������� �������.
    ENumSignalsInSec::E enumSignalsInSec;      // ������������ ���������� ������� � �������.
    Channel::E          last_affected_channel; // ����� �������� ����� ���������� ������, ������� ��������� �������.
                                               // ����� ��� ����, ����� �����, ����� ������ �������� �������.
    ModeAccumulation::E mode_acum;             // ����� ����� ���������� ��������.
    AltMarkers::E       alt_markers;           // ����� ����������� �������������� ������� �������� ��������.
    MenuAutoHide::E     menuAutoHide;          // ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    bool                showFullMemoryWindow;  // ���������� �� ���� ������ ������ ������. \todo �� ������������.
    bool                showStringNavigation;  // ���������� �� ������ �������� ��������� ����..
    LinkingRShift::E    linking_rshift;        // ��� �������� �������� �� ���������.

    // ���������� ����� ������ � ��������� ����� �� ������.
    static void PointsOnDisplay(int *firstPoint, int *lastPoint);

    // ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
    static bool IsSeparate();
};
