// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "Display/Warnings.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Pages/HelpContent.h"
#include "Panel/Panel.h"
#include <cstring>


// Если произошло короткое нажатие кнопки, то здесь хранится имя этой кнопки до обработки этого  нажатия.
static Key::E shortPressureButton = Key::None;
// Если произошло длинное нажатие кнопки, то здесь хранится имя этой кнопки до обработки этого нажатия.
static Key::E longPressureButton = Key::None;
// При нажатии кнопки её имя записывается в эту переменную и хранится там до обратоки события нажатия кнопки.
static Key::E pressButton = Key::None;
// При отпускании кнопки её имя записывается в эту переменную и хранится там до обработки  события отпускания кнопки.
static Key::E releaseButton = Key::None;
// Угол, на который нужно повернуть ручку УСТАНОВКА - величина означает количество щелчков,
// знак - направление - "-" - влево, "+" - вправо
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
    led_RegSet.Switch();
    
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
            "Кнопка МЕНЮ выполняет следующие функции:\n"
            "1. При закрытом меню нажатие либо нажатие с удержанием в течение 0.5с открывает меню.\n"
            "2. При открытом меню удержание кнопки в течение 0.5с закрывает меню.\n"
            "3. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Закрывать\" текущей становится страница предыдущего уровня "
            "меню. Если текущей является корневая страница, меню закрывается.\n"
            "4. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Переключать\" текущей становится страница текущего уровня "
            "меню. Если текущая страница последняя в текущем уровне, происходит переход на предыдущий уровень меню.\n"
            "5. Если меню находится в режиме малых кнопок, то нажатие закрывает страницу."
            :
            "MENU button performs the following functions:\n"
            "1. At the closed menu pressing or pressing with deduction during 0.5s opens the menu.\n"
            "2. At the open menu deduction of the button during 0.5s closes the menu.\n"
#ifdef __ARMCC_VERSION
#pragma push
#pragma diag_suppress 192
#endif
            "3. At control \"SERVICE\x99Mode btn MENU\x99\x43lose\" current becomes the page of the previous level of "
                "the menu. If the root page is current, the menu is closed.\n"
#ifdef __ARMCC_VERSION
#pragma pop
#endif
            "4. At control \"SERVICE\x99Mode btn MENU\x99Toggle\" current becomes the page of the current level of the "
            "menu. If the current page the last in the current level, happens transition to the previous level of the "
            "menu.\n"
            "5. If the menu is in the mode of small buttons, pressing closes the page.";

    } else if (button == Key::Cursors)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "Кнопка КУРСОРЫ открывает меню курсорных измерений."
            :
            "КУРСОРЫ button to open the menu cursor measurements.";
    }
    else if (button == Key::Display)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "Кнопка ДИСПЛЕЙ открывает меню настроек дисплея."
            :
            "DISPLAY button opens the display settings menu.";
    }
    else if (button == Key::Memory)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. При настройке \"ПАМЯТЬ\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Меню\" открывает меню работы с памятью.\n"
            "2. При настройке \"ПАМЯТь\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Сохранение\" сохраняет сигнал на флеш-диск."
            :
            "1. When setting \"MEMORY-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"MEMORY-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (button == Key::Measures)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "Кнопка ИЗМЕР открывает меню автоматических измерений."
            :
            "ИЗМЕР button opens a menu of automatic measurements.";
    }
    else if (button == Key::Help)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. Кнопка ПОМОЩЬ открывает меню помощи.\n"
            "2. Нажатие и удержание кнопки ПОМОЩЬ в течение 0.5с включает и отключает режим вывода подсказок."
            :
            "1. ПОМОЩЬ button opens the help menu.\n"
            "2. Pressing and holding the ПОМОЩЬ for 0.5s enables and disables output mode hints.";
    }
    else if (button == Key::Service)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "Кнопка СЕРВИС открывает меню сервисных возможностей."
            :
            "СЕРВИС button opens a menu of service options.";
    }
    else if (button == Key::Start)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "Кнопка ПУСК/СTOП запускает и останавливает процесс сбора информации."
            :
            "ПУСК/СTOП button starts and stops the process of gathering information.";
    }
    else if (button == Key::ChannelA)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. Кнопка КАНАЛ1 открывает меню настроек канала 1.\n"
            "2. Нажатие и удержание кнопки КАНАЛ1 в течение 0.5с устанавливает смещение канала 1 по вертикали 0В."
            :
            "1. КАНАЛ1 button opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the button КАНАЛ1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (button == Key::ChannelB)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. Кнопка КАНАЛ2 открывает меню настроек канала 2.\n"
            "2. Нажатие и удержание кнопки КАНАЛ2 в течение 0.5с устанавливает смещение канала 2 по вертикали 0В."
            :
            "1. КАНАЛ2 button opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the button КАНАЛ2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (button == Key::Time)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. Кнопка РАЗВ открывает меню настроек развертки.\n"
            "2. Нажатие и удержание кнопки РАЗВ в течение 0.5с устанавливает смещение по горизонтали 0с."
            :
            "1. РАЗВ button open the settings menu sweep.\n"
            "2. Pressing and holding the button РАЗВ for 0.5s Is the offset horizontal 0s.";
    }
    else if (button == Key::Trig)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. Кнопка СИНХР открывает меню настроек синхронизации.\n"
            "2. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит "
            "СИНХР\x99Автоуровень\" производит автоматическую настройку уровня синхронизации.\n"
            "3. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит "
            "СИНХР\x99Сброс уровня\" устанавливает уровень синхронизации 0В."
            :
            "1. СИНХР button opens a menu settings synchronization.\n"
#ifdef __ARMCC_VERSION
#pragma push
#pragma diag_suppress 192
#endif
            "2. Pressing and holding the button СИНХР for 0.5s when setting \"SERVICE\x99Mode long "
            "TRIG\x99\x41utolevel\" automatically adjust the trigger level.\n"
#ifdef __ARMCC_VERSION
#pragma pop
#endif
            "3. Pressing and holding the button СИНХР for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig "
            "level\" sets the trigger level 0V.";
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
            Warnings::ShowWarningGood(Warning::MenuDebugEnabled);
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
    if(set.display.menu_auto_hide.Time() == 0)
    {
        Timer::Disable(TypeTimer::MenuAutoHide);
    }
    else
    {
        Timer::Enable(TypeTimer::MenuAutoHide, set.display.menu_auto_hide.Time(), OnTimerAutoHide);
    }
}


Text Menu::StringNavigation()
{
    Text result;

    if(set.display.show_string_navigation && IsShown())
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
    Hide();
    Timer::Disable(TypeTimer::MenuAutoHide);
}


void Menu::ProcessingShortPressureButton()
{
    if(shortPressureButton != Key::None)
    {
        LOG_WRITE(" ");

        if (shortPressureButton == Key::Memory &&
            set.memory.mode_button_memory.IsSave() &&
            FDrive::isConnected)
        {
            PageMemory::exitFromModeSetNameTo = IsShown() ? RETURN_TO_MAIN_MENU : RETURN_TO_DISABLE_MENU;
            PageMemory::PageExternal::SaveSignalToFlashDrive();
            shortPressureButton = Key::None;
            return;
        }
        Display::Redraw();
        SetAutoHide(true);

        Key::E button = shortPressureButton;

        do
        {
            if(button == Key::Menu)            // Если нажата кнопка МЕНЮ и мы не находимся в режме настройки измерений.
            {
                if(!IsShown())
                {
                    Show();
                }
                else
                {
                    CloseOpenedItem();
                }
            }
            else if (IsShown() && Key::IsFunctionalButton(button)) // Если меню показано и нажата функциональная клавиша
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
            else                                                        // Если меню не показано.
            {
                NamePage::E name = ((const Page *)Item::Opened())->GetName();
                if(button == Key::ChannelA && name == NamePage::ChannelA)
                {
                    set.chan[ChA].enable = !ChA.IsEnabled();
                    PageChannelA::OnChanged_Input(true);
                }
                else if(button == Key::ChannelB && name == NamePage::ChannelB)
                {
                    set.chan[ChB].enable = !ChB.IsEnabled();
                    PageChannelB::OnChanged_Input(true);
                }
                else
                {
                    const Item *page = PageForButton(button);
                    if (page)
                    {
                        page->SetCurrent(true);
                        page->Open();
                        Show();
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
            TrigLev::Set(TrigSource::Get(), TrigLev::ZERO);
        }
        else if(longPressureButton == Key::ChannelA)
        {
            RShift::Set(ChA, RShift::ZERO);
        }
        else if(longPressureButton == Key::ChannelB)
        {
            RShift::Set(ChB, RShift::ZERO);
        }
        else if(longPressureButton == Key::Menu)
        {
            ChangeShowing();
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
            else if (item->IsPage() || item->IsIP() || item->IsMAC() || item->IsChoice() || item->IsChoiceReg() ||
                     item->IsGovernor())
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
    if (pressButton == Key::Start && !set.memory.mode_work.IsLatest())
    {
        FPGA::CurrentStateWork().IsStop() ? FPGA::Start() : FPGA::Stop();
    } 

    pressButton = Key::None;
}


void Menu::ProcessingReleaseButton()
{
    if((releaseButton >= Key::F1 && releaseButton <= Key::F5) || pressButton == Key::Menu)
    {
        releaseButton = Key::None;
    }
}


void Menu::OpenItemTime()
{
    Warnings::ShowWarningGood(Warning::TimeNotSet);
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


void Menu::ChangeShowing()
{
    if (IsShown())
    {
        Hide();
    }
    else
    {
        Show();
    }
}


void Menu::Hide()
{
    set.menu.isShown = 0U;
}


void Menu::Show()
{
    set.menu.isShown = 1U;
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
