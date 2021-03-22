#pragma once
#include "common/Display/Colors_.h"


#define NUM_AVE                     (1 << set.display.enum_ave)                 // ���������� ����������.

#define ENUM_MIN_MAX                (::set.display.enumMinMax)                  // SettingsDisplay.enumMinMax
#define NUM_MIN_MAX                 (1 << ENUM_MIN_MAX)                         // ���������� ��������� �������� ��� ������� ��������� � ����������.

#define SHOW_STRING_NAVIGATION      (set.display.showStringNavigation)          // SettingsDisplay.showStringNavigation

// ��������� �����������
struct SettingsDisplay
{
    int16               time_show_levels;      // �����, � ������� �������� ����� ���������� ������ ��������.
    int16               shift_in_memory;       // ���������� �������� ������ ���� ����� ������������ �������� �����
                                               // ������. ����� ��� ����������� ����������� ������� � ����.
    int16               time_messages;         // ����� � ��������, �� ������� ��������� ������� �� ������.
    int16               brightness;            // ������� �������.
    int16               brightness_grid;       // ������� ����� �� 0 �� 100.
    uint                colors[16];            // �����.
    ModeDrawSignal::E   mode_draw_signal;      // ����� ��������� �������.
    TypeGrid::E         typeGrid;              // ��� �����.
    ENumAccumulation::E enum_accum;            // ������������ ���������� ������� �� ������.
    ENumAveraging::E    enum_ave;              // ������������ ���������� �������.
    ModeAveraging::E    mode_ave;              // ��� ���������� �� ����������.
    ENumMinMax::E       enumMinMax;            // ����� ��������� ��� ����������� ��������� � ����������.
    Smoothing::E        smoothing;             // ����� ����� ��� ����������� �������.
    ENumSignalsInSec::E enum_fps;              // ������������ ���������� ������� � �������.
    Channel::E          last_affected_channel; // ����� �������� ����� ���������� ������, ������� ��������� �������.
                                               // ����� ��� ����, ����� �����, ����� ������ �������� �������.
    ModeAccumulation::E mode_acum;             // ����� ����� ���������� ��������.
    AltMarkers::E       alt_markers;           // ����� ����������� �������������� ������� �������� ��������.
    MenuAutoHide::E     menu_auto_hide;        // ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    bool                showFullMemoryWindow;  // ���������� �� ���� ������ ������ ������. \todo �� ������������.
    bool                showStringNavigation;  // ���������� �� ������ �������� ��������� ����..
    LinkingRShift::E    linking_rshift;        // ��� �������� �������� �� ���������.

    // ���������� ����� ������ � ��������� ����� �� ������.
    static void PointsOnDisplay(int *firstPoint, int *lastPoint);

    // ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
    static bool IsSeparate();
};
