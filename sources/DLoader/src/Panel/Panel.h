#pragma once 


#include "defines.h"
#include "Controls.h"


void    Panel_Update(void);
void    Panel_TransmitData(uint16 data);            // Передать даннные в мк панели управления.
void    Panel_Disable(void);                        // В отлюченном режиме панель лишь обновляет состояние переменной pressedButton, не выполняя больше никаких действий.
void    Panel_Enable(void);
PanelButton Panel_WaitPressingButton(void);       // Ожидать нажатие клавиши.
PanelButton Panel_PressedButton(void);

struct Panel
{
    static bool   ProcessingCommandFromPIC(uint16 command);
    static uint16 NextData(void);
};
