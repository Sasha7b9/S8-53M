#pragma once
#include "defines.h"
#include "Panel/Controls.h"
#include "Settings/SettingsCursors.h"


#define COMMON_BEGIN_SB_EXIT  0, "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu"


const void* PageForButton(PanelButton::E button);   // ���������� �������� ����, ������� ������ ����������� �� ������� ������ button.
bool IsMainPage(const void *item);                        // ���������� true, ���� item - ����� ������� �������� ����.


void DrawMenuCursVoltage(int x, int y, bool top, bool bottom);
void CalculateConditions(int16 pos0, int16 pos1, CursCntrl::E cursCntrl, bool *cond0, bool *cond1);    // ������������ ������� ��������� ��� ����� ������ ���������� ������� ���������.
void DrawMenuCursTime(int x, int y, bool left, bool right);
void DrawSB_Exit(int x, int y);
