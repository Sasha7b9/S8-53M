#pragma once
#include "MenuItems.h"


void    IPaddress_GetNumPosIPvalue(int *numIP, int *selPos);    // ���������� ����� �������� ����� (4 - ����� �����) � ����� ������� ������� � �����.

void    MACaddress_ChangeValue(MACaddress *mac, int delta);

void    ItemTime_SetOpened(Time *time);
void    ItemTime_SetNewTime(const Time *time);
void    ItemTime_SelectNextPosition(Time *time);
void    ItemTime_IncCurrentPosition(const Time *time);
void    ItemTime_DecCurrentPosition(const Time *time);

void    GovernorColor_ChangeValue(GovernorColor *governor, int delta);  // �������� ������� ����� � governor

extern int8 gCurDigit;
