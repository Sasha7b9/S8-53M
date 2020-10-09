#pragma once
#include "MenuItems.h"


int16   Governor_NextValue(const Governor *governor);           // ¬озвращает следующее большее значение, которое может прин€ть governor.
int16   Governor_PrevValue(const Governor *governor);           // ¬озвращает следующее меньшее значение, которое может прин€ть governor.

void    IPaddress_NextPosition(const IPaddress *ip);            // ѕри открытом элементе переставл€ет курсор на следующую позицию
void    IPaddress_ChangeValue(IPaddress *ip, int delta);        // »змен€ет значение в текущей позиции при открытом элементе
void    IPaddress_GetNumPosIPvalue(int *numIP, int *selPos);    // ¬озвращает номер текущего байта (4 - номер порта) и номер текущей позиции в байте.

void    MACaddress_ChangeValue(MACaddress *mac, int delta);

void    ItemTime_SetOpened(Time *time);
void    ItemTime_SetNewTime(const Time *time);
void    ItemTime_SelectNextPosition(Time *time);
void    ItemTime_IncCurrentPosition(const Time *time);
void    ItemTime_DecCurrentPosition(const Time *time);

void    GovernorColor_ChangeValue(GovernorColor *governor, int delta);  // »зменить €ркость цвета в governor

extern int8 gCurDigit;
