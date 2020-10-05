#pragma once 
#include "defines.h"
#include "Controls.h"


void    Panel_Update(void);
void    Panel_TransmitData(uint16 data);            // �������� ������� � �� ������ ����������.
void    Panel_Disable(void);                        // � ���������� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� �������� ������ ������� ��������.
void    Panel_Enable(void);
PanelButton::E Panel_WaitPressingButton(void);       // ������� ������� �������.


struct Panel
{
    static bool           ProcessingCommandFromPIC(uint16 command);
    static uint16         NextData();
    static PanelButton::E PressedButton();
};
