#include "Controls.h"


static const char* namesButtons[] = 
{
    "PanelButton::Empty",
    "PanelButton::Channel0",
    "PanelButton::Service",
    "PanelButton::Channel1",
    "PanelButton::Display",
    "PanelButton::Time",
    "PanelButton::Memory",
    "B_Sinchro",
    "PanelButton::Start",
    "PanelButton::Cursors",
    "PanelButton::Measures",
    "B_Power",
    "B_Info",
    "PanelButton::Menu",
    "PanelButton::F1",
    "PanelButton::F2",
    "PanelButton::F3",
    "PanelButton::F4",
    "PanelButton::F5"
};


const char* NameButton(PanelButton::E button) 
{
    return namesButtons[button];
}
