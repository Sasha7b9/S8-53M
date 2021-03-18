#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "common/Hardware/Timer_.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Pages/HelpContent.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <cstring>


// ���� ��������� �������� ������� ������, �� ����� �������� ��� ���� ������ �� ��������� �����  �������.
static Key::E shortPressureButton = Key::None;
// ���� ��������� ������� ������� ������, �� ����� �������� ��� ���� ������ �� ��������� ����� �������.
static Key::E longPressureButton = Key::None;
// ��� ������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� �������� ������� ������� ������.
static Key::E pressButton = Key::None;
// ��� ���������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� ���������  ������� ���������� ������.
static Key::E releaseButton = Key::None;
// ����, �� ������� ����� ��������� ����� ��������� - �������� �������� ���������� �������, ���� - ����������� - "-" - �����, "+" - ������
static int angleRegSet = 0;

static const int stepAngleRegSet = 2;

bool Menu::showHelpHints = false;
bool Menu::showDebugMenu = false;
bool Menu::needClosePageSB = false;

#define SIZE_BUFFER_FOR_BUTTONS 5
static Key::E bufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {Key::None, Key::None, Key::None, Key::None, Key::None};
static const Key::E sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {Key::F5, Key::F4, Key::F3, Key::F2, Key::F1};


void Menu::Update()
{
    ProcessingShortPressureButton();
    ProcessingLongPressureButton();
    ProcessingRegulatorSet();
    ProcessingPressButton();
    ProcessingReleaseButton();
    SwitchSetLED();
    
    if(FDrive::needOpenFileMananger)
    {
        FDrive::needOpenFileMananger = false;
        OpenFileManager();       
    }
};


void Menu::Event::ShortPressureButton(Key::E button)
{
    if (!showHelpHints)
    {
        if(button == Key::Help)
        {
            showHelpHints = true;
            PageHelpContent::stringForHint = 0;
            PageHelpContent::itemHint = 0;
        }
        shortPressureButton = button;
    }
};



void Menu::Event::LongPressureButton(Key::E button)
{
    if (!showHelpHints)
    {
        longPressureButton = button;
        Display::Redraw();
    }
};


void Menu::ProcessButtonForHint(Key::E button)
{
    if (button == Key::Menu)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ���� ��������� ��������� �������:\n"
            "1. ��� �������� ���� ������� ���� ������� � ���������� � ������� 0.5� ��������� ����.\n"
            "2. ��� �������� ���� ��������� ������ � ������� 0.5� ��������� ����.\n"
            "3. ��� ��������� \"������\x99����� �� ����\x99���������\" ������� ���������� �������� ����������� ������ ����. ���� ������� �������� �������� ��������, ���� �����������.\n"
            "4. ��� ��������� \"������\x99����� �� ����\x99�����������\" ������� ���������� �������� �������� ������ ����. ���� ������� �������� ��������� � ������� ������, ���������� ������� �� ���������� ������� ����.\n"
            "5. ���� ���� ��������� � ������ ����� ������, �� ������� ��������� ��������."
            :
        "MENU button performs the following functions:\n"
            "1. At the closed menu pressing or pressing with deduction during 0.5s opens the menu.\n"
            "2. At the open menu deduction of the button during 0.5s closes the menu.\n"
#ifndef WIN32
#pragma push
#pragma diag_suppress 192
#endif
            "3. At control \"SERVICE\x99Mode btn MENU\x99\x43lose\" current becomes the page of the previous level of the menu. If the root page is current, the menu is closed.\n"
#ifndef WIN32
#pragma pop
#endif
            "4. At control \"SERVICE\x99Mode btn MENU\x99Toggle\" current becomes the page of the current level of the menu. If the current page the last in the current level, happens transition to the previous level of the menu.\n"
            "5. If the menu is in the mode of small buttons, pressing closes the page.";

    } else if (button == Key::Cursors)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ������� ��������� ���� ��������� ���������."
            :
            "������� button to open the menu cursor measurements.";
    }
    else if (button == Key::Display)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ������� ��������� ���� �������� �������."
            :
            "DISPLAY button opens the display settings menu.";
    }
    else if (button == Key::Memory)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����\" ��������� ���� ������ � �������.\n"
            "2. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����������\" ��������� ������ �� ����-����."
            :
            "1. When setting \"MEMORY-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"MEMORY-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (button == Key::Measures)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ����� ��������� ���� �������������� ���������."
            :
            "����� button opens a menu of automatic measurements.";
    }
    else if (button == Key::Help)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ������ ������ ��������� ���� ������.\n"
            "2. ������� � ��������� ������ ������ � ������� 0.5� �������� � ��������� ����� ������ ���������."
            :
            "1. ������ button opens the help menu.\n"
            "2. Pressing and holding the ������ for 0.5s enables and disables output mode hints.";
    }
    else if (button == Key::Service)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ������ ��������� ���� ��������� ������������."
            :
            "������ button opens a menu of service options.";
    }
    else if (button == Key::Start)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ����/�TO� ��������� � ������������� ������� ����� ����������."
            :
            "����/�TO� button starts and stops the process of gathering information.";
    }
    else if (button == Key::ChannelA)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ������ �����1 ��������� ���� �������� ������ 1.\n"
            "2. ������� � ��������� ������ �����1 � ������� 0.5� ������������� �������� ������ 1 �� ��������� 0�."
            :
            "1. �����1 button opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the button �����1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (button == Key::ChannelB)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ������ �����2 ��������� ���� �������� ������ 2.\n"
            "2. ������� � ��������� ������ �����2 � ������� 0.5� ������������� �������� ������ 2 �� ��������� 0�."
            :
            "1. �����2 button opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the button �����2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (button == Key::Time)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ������ ���� ��������� ���� �������� ���������.\n"
            "2. ������� � ��������� ������ ���� � ������� 0.5� ������������� �������� �� ����������� 0�."
            :
            "1. ���� button open the settings menu sweep.\n"
            "2. Pressing and holding the button ���� for 0.5s Is the offset horizontal 0s.";
    }
    else if (button == Key::Trig)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ������ ����� ��������� ���� �������� �������������.\n"
            "2. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99�����������\" ���������� �������������� ��������� ������ �������������.\n"
            "3. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99����� ������\" ������������� ������� ������������� 0�."
            :
            "1. ����� button opens a menu settings synchronization.\n"
#ifndef WIN32
#pragma push
#pragma diag_suppress 192
#endif
            "2. Pressing and holding the button ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99\x41utolevel\" automatically adjust the trigger level.\n"
#ifndef WIN32
#pragma pop
#endif
            "3. Pressing and holding the button ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig level\" sets the trigger level 0V.";
    }
    else
    {
        shortPressureButton = button;
    }
}


void Menu::Event::PressButton(Key::E button)
{
    Sound::ButtonPress();
    if (showHelpHints)
    {
        ProcessButtonForHint(button);
        return;
    }

    if (!IsShown())
    {
        for (int i = SIZE_BUFFER_FOR_BUTTONS - 1; i > 0; i--)
        {
            bufferForButtons[i] = bufferForButtons[i - 1];
        }
        bufferForButtons[0] = button;
      
        if (std::memcmp(bufferForButtons, sampleBufferForButtons, SIZE_BUFFER_FOR_BUTTONS * sizeof(Key::E)) == 0)
        {
            showDebugMenu = true;
            Display::ShowWarningGood(Warning::MenuDebugEnabled);
        }
    }
    pressButton = button;
};


void Menu::Event::ReleaseButton(Key::E button)
{
    Sound::ButtonRelease();
    if (!showHelpHints)
    {
        releaseButton = button;
    }
};


void Menu::Event::RotateRegSetRight()
{   
    if (!showHelpHints)
    {
        angleRegSet++;
        Display::Redraw();
    }
};


void Menu::Event::RotateRegSetLeft()
{
    if (!showHelpHints)
    {
        angleRegSet--;
        Display::Redraw();
    }
};


Item* Menu::ItemUnderKey()
{
    return nullptr;
};


void Menu::SetAutoHide(bool)
{
    if(!IsShown())
    {
        return;
    }
    if(MenuAutoHide::Time() == 0)
    {
        Timer::Disable(TypeTimer::MenuAutoHide);
    }
    else
    {
        Timer::Enable(TypeTimer::MenuAutoHide, MenuAutoHide::Time(), OnTimerAutoHide);
    }
}


String Menu::StringNavigation()
{
    String result;

    if(SHOW_STRING_NAVIGATION && IsShown())
    {
        pchar titles[10] = {0};
        int numTitle = 0;
        const Item *item = Item::Opened();

        if(IsMainPage(item))
        {
            return result;
        }

        while(!IsMainPage(item))
        {
            titles[numTitle++] = item->Title();
            item = item->Keeper();
        }

        for(int i = 9; i >= 0; i--)
        {
            if(titles[i])
            {
                result.Append(titles[i]);

                if(i != 0)
                {
                    result.Append(" - ");
                }
            }
        }
    }

    return result;
}


void Menu::OnTimerAutoHide()
{
    Show(false);
    Timer::Disable(TypeTimer::MenuAutoHide);
}


void Menu::ProcessingShortPressureButton()
{
    if(shortPressureButton != Key::None)
    {
        if (shortPressureButton == Key::Memory && MODE_BTN_MEMORY_IS_SAVE && FDrive::isConnected)
        {
            PageMemory::exitFromModeSetNameTo = IsShown() ? RETURN_TO_MAIN_MENU : RETURN_TO_DISABLE_MENU;
            Memory_SaveSignalToFlashDrive();
            shortPressureButton = Key::None;
            return;
        }
        Display::Redraw();
        SetAutoHide(true);

        Key::E button = shortPressureButton;

        do
        {
            if(button == Key::Menu)                                   // ���� ������ ������ ���� � �� �� ��������� � ����� ��������� ���������.
            {
                if(!IsShown())
                {
                    Show(true);
                }
                else
                {
                    CloseOpenedItem();
                }
            }
            else if (IsShown() && Key::IsFunctionalButton(button))       // ���� ���� �������� � ������ �������������� �������
            {
                Item *item = ItemUnderButton(button);
                if (showHelpHints)
                {
                    PageHelpContent::SetItemForHint(item);
                }
                else
                {
                    item->ShortPress();
                }
            }
            else                                                        // ���� ���� �� ��������.
            {
                NamePage::E name = ((const Page *)Item::Opened())->GetName();
                if(button == Key::ChannelA && name == NamePage::ChannelA)
                {
                    SET_ENABLED_A = !sChannel_Enabled(ChA);
                    PageChannelA::OnChanged_Input(true);
                }
                else if(button == Key::ChannelB && name == NamePage::ChannelB)
                {
                    SET_ENABLED_B = !sChannel_Enabled(ChB);
                    PageChannelB::OnChanged_Input(true);
                }
                else
                {
                    const Item *page = PageForButton(button);
                    if (page)
                    {
                        page->SetCurrent(true);
                        page->Open(true);
                        Show(true);
                    }
                }
            }
        } while (false);

        shortPressureButton = Key::None;
    }
}


void Menu::ProcessingLongPressureButton()
{
    if(longPressureButton != Key::None)
    {
        Display::Redraw();
        SetAutoHide(true);

        if(longPressureButton == Key::Time)
        {
            TShift::Set(0);
        }
        else if(longPressureButton == Key::Trig)
        {
            TrigLev::Set(TRIG_SOURCE, TrigLevZero);
        }
        else if(longPressureButton == Key::ChannelA)
        {
            RShift::Set(ChA, RShiftZero);
        }
        else if(longPressureButton == Key::ChannelB)
        {
            RShift::Set(ChB, RShiftZero);
        }
        else if(longPressureButton == Key::Menu)
        {
                Show(!IsShown());
        }
        else if(IsShown() && Key::IsFunctionalButton(longPressureButton))
        {
            Item *item = ItemUnderButton(longPressureButton);
            if (item->IsActive())
            {
                item->LongPress();
            }
        }
        longPressureButton = Key::None;
    }
}


void Menu::ProcessingRegulatorSet()
{
    if (angleRegSet == 0)
    {
        return;
    }

    if (IsShown() || !Item::Opened()->IsPage())
    {
        const Item *item = CurrentItem();
        if (Item::Opened()->IsPage() && (item->IsChoiceReg() || item->IsGovernor() || item->IsIP() || item->IsMAC()))
        {
            if (angleRegSet > stepAngleRegSet || angleRegSet < -stepAngleRegSet)
            {
                item->Change(angleRegSet);
                angleRegSet = 0;
            }
            return;
        }
        else
        {
            item = Item::Opened();
            if (IsMinimize())
            {
                CurrentPageSBregSet(angleRegSet);
            }
            else if (item->IsPage() || item->IsIP() || item->IsMAC() || item->IsChoice() || item->IsChoiceReg() || item->IsGovernor())
            {
                if (item->ChangeOpened(angleRegSet))
                {
                    angleRegSet = 0;
                }
                return;
            }
            else if (item->IsGovernorColor())
            {
                item->Change(angleRegSet);
            }
            else if (item->IsTime())
            {
                angleRegSet > 0 ? ((TimeItem *)item)->IncCurrentPosition() : ((TimeItem *)item)->DecCurrentPosition();
            }
        }
    }

    angleRegSet = 0;
}


void Menu::ProcessingPressButton()
{
    if (pressButton == Key::Start && !MODE_WORK_IS_LATEST)
    {
        FPGA::OnPressStartStop();
    } 

    pressButton = Key::None;
}


void Menu::ProcessingReleaseButton()
{
    if(releaseButton >= Key::F1 && releaseButton <= Key::F5 || pressButton == Key::Menu)
    {
        releaseButton = Key::None;
    }
}


void Menu::OpenItemTime()
{
    Display::ShowWarningGood(Warning::TimeNotSet);
    Event::ShortPressureButton(Key::Service);
    Update();
    Display::Update();
    for (int i = 0; i < 2; i++)
    {
        Event::RotateRegSetRight();
        Update();
        Display::Update();
    }
    Event::ShortPressureButton(Key::F4);
    Update();
    Display::Update();
}


bool Menu::NeedForFireSetLED()    // ���������� true, ���� �������� ��������� ������ ������
{
    if (!IsShown())
    {
        return false;
    }
    NamePage::E name = GetNameOpenedPage();

    if (name == NamePage::SB_MeasTuneMeas && MEAS_NUM == MeasuresNumber::_1 && !PageMeasures::choiceMeasuresIsActive)
    {
        return false;
    }

    if (
        name == NamePage::SB_MathCursorsFFT                          ||
        name == NamePage::SB_MeasTuneMeas                            ||
        name == NamePage::SB_MemLatest                               || 
        name == NamePage::SB_MemInt                                  ||
        (name == NamePage::SB_MathFunction && !DISABLED_DRAW_MATH)   ||
        name == NamePage::SB_Curs && sCursors_NecessaryDrawCursors() //-V648
        )
    {
        return true;
    }
    
    if (CurrentItem()->IsGovernor() || CurrentItem()->IsChoiceReg())
    {
        return true;
    }

    if (Item::Opened()->IsChoice()  ||
        (Item::Opened()->IsPage() && (Item::Opened()->ReinterpretToPage()->NumSubPages() > 1)))
    {
        return true;
    }
    
    return false;
}


void Menu::SwitchSetLED()
{
    static bool first = true;
    static bool prevState = false;  // true - �����, false - �� �����

    bool state = NeedForFireSetLED();

    if (first)
    {
        first = false;
        Panel::EnableLEDRegSet(state);
        prevState = state;
    }
    else if (prevState != state)
    {
        Panel::EnableLEDRegSet(state);
        prevState = state;
    }
}


void Menu::OpenFileManager()
{
    angleRegSet = 0;
    for (int i = 0; i < 10; i++)
    {
        Event::ShortPressureButton(Key::Menu);
        Update();
        Display::Update();
    }

    if (!IsShown())
    {
        Event::ShortPressureButton(Key::Menu);
        Update();
        Display::Update();
    }

    for (int i = 0; i < 5 * stepAngleRegSet + 1; i++)
    {
        Event::RotateRegSetLeft();
        Update();
        Display::Update();
    }

    angleRegSet = 0;

    for (int i = 0; i < 2 * stepAngleRegSet + 1; i++)
    {
        Event::RotateRegSetRight();
        Update();
        Display::Update();
    }

    angleRegSet = 0;

    Event::ShortPressureButton(Key::F2);
    Update();
    Display::Update();

    Event::ShortPressureButton(Key::F4);
    Update();
    Display::Update();

    for (int i = 0; i < stepAngleRegSet + 1; i++)
    {
        Event::RotateRegSetLeft();
        Update();
        Display::Update();
    }

    for (int i = 0; i < 2; i++)
    {
        Event::ShortPressureButton(Key::F1);
        Update();
        Display::Update();
    }
}


bool Menu::IsShown()
{
    return (set.menu.isShown == 1);
}


void Menu::Show(bool show)
{
    set.menu.isShown = show ? 1U : 0U;
    SetAutoHide(true);
}


bool Menu::IsMinimize()
{
    return Item::Opened()->IsPage() && ((const Page *)Item::Opened())->GetName() >= NamePage::SB_Curs;
}


void Menu::CurrentPageSBregSet(int angle)
{
    const Page *page = Item::Opened()->ReinterpretToPage();
    if (page->OwnData()->funcRegSetSB)
    {
        page->OwnData()->funcRegSetSB(angle);
    }
}
