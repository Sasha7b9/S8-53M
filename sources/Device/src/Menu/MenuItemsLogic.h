#pragma once
#include "MenuItems.h"


void    ItemTime_IncCurrentPosition(const Time *time);
void    ItemTime_DecCurrentPosition(const Time *time);

void    GovernorColor_ChangeValue(GovernorColor *governor, int delta);  // �������� ������� ����� � governor

extern int8 gCurDigit;
