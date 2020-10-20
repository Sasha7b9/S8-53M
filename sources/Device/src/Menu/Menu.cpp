#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Pages/PageMeasures.h"
#include "Menu/Pages/PageMemory.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <cstring>


extern void OnChanged_InputA(bool active);
extern void OnChanged_InputB(bool active);
                                                
// Если произошло короткое нажатие кнопки, то здесь хранится имя этой кнопки до обработки этого  нажатия.
static Key::E shortPressureButton = Key::None;
// Если произошло длинное нажатие кнопки, то здесь хранится имя этой кнопки до обработки этого нажатия.
static Key::E longPressureButton = Key::None;
// При нажатии кнопки её имя записывается в эту переменную и хранится там до обратоки события нажатия кнопки.
static Key::E pressButton = Key::None;
// При отпускании кнопки её имя записывается в эту переменную и хранится там до обработки  события отпускания кнопки.
static Key::E releaseButton = Key::None;
// Угол, на который нужно повернуть ручку УСТАНОВКА - величина означает количество щелчков, знак - направление - "-" - влево, "+" - вправо
static int angleRegSet = 0;

static const int stepAngleRegSet = 2;

Item *Menu::itemUnderKey = nullptr;

#define SIZE_BUFFER_FOR_BUTTONS 5
static Key::E bufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {Key::None, Key::None, Key::None, Key::None, Key::None};
static const Key::E sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {Key::F5, Key::F4, Key::F3, Key::F2, Key::F1};


void Menu::UpdateInput()
{
    ProcessingShortPressureButton();
    ProcessingLongPressureButton();
    ProcessingRegulatorSet();
    ProcessingPressButton();
    ProcessingReleaseButton();
    SwitchSetLED();
    
    if(NEED_OPEN_FILE_MANAGER)
    {
        NEED_OPEN_FILE_MANAGER = 0;
        OpenFileManager();       
    }
};


void Menu::ShortPressureButton(Key::E button)
{
    if (SHOW_HELP_HINTS == 0)
    {
        if(button == Key::Help)
        {
            SHOW_HELP_HINTS++;
            gStringForHint = 0;
            gItemHint = 0;
        }
        shortPressureButton = button;
    }
};



void Menu::LongPressureButton(Key::E button)
{
    if (SHOW_HELP_HINTS == 0)
    {
        longPressureButton = button;
        Display::Redraw();
    }
};


void Menu::ProcessButtonForHint(Key::E button)
{
    if (button == Key::Menu)
    {
        gStringForHint = set.common.lang == Language::Russian ?
            "Кнопка МЕНЮ выполняет следующие функции:\n"
            "1. При закрытом меню нажатие либо нажатие с удержанием в течение 0.5с открывает меню.\n"
            "2. При открытом меню удержание кнопки в течение 0.5с закрывает меню.\n"
            "3. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Закрывать\" текущей становится страница предыдущего уровня меню. Если текущей является корневая страница, меню закрывается.\n"
            "4. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Переключать\" текущей становится страница текущего уровня меню. Если текущая страница последняя в текущем уровне, происходит переход на предыдущий уровень меню.\n"
            "5. Если меню находится в режиме малых кнопок, то нажатие закрывает страницу."
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
        gStringForHint = LANG_RU ? 
            "Кнопка КУРСОРЫ открывает меню курсорных измерений."
            :
            "КУРСОРЫ button to open the menu cursor measurements.";
    }
    else if (button == Key::Display)
    {
        gStringForHint = LANG_RU ?
            "Кнопка ДИСПЛЕЙ открывает меню настроек дисплея."
            :
            "DISPLAY button opens the display settings menu.";
    }
    else if (button == Key::Memory)
    {
        gStringForHint = LANG_RU ?
            "1. При настройке \"ПАМЯТЬ\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Меню\" открывает меню работы с памятью.\n"
            "2. При настройке \"ПАМЯТь\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Сохранение\" сохраняет сигнал на флеш-диск."
            :
            "1. When setting \"MEMORY-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"MEMORY-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (button == Key::Measures)
    {
        gStringForHint = LANG_RU ?
            "Кнопка ИЗМЕР открывает меню автоматических измерений."
            :
            "ИЗМЕР button opens a menu of automatic measurements.";
    }
    else if (button == Key::Help)
    {
        gStringForHint = LANG_RU ?
            "1. Кнопка ПОМОЩЬ открывает меню помощи.\n"
            "2. Нажатие и удержание кнопки ПОМОЩЬ в течение 0.5с включает и отключает режим вывода подсказок."
            :
            "1. ПОМОЩЬ button opens the help menu.\n"
            "2. Pressing and holding the ПОМОЩЬ for 0.5s enables and disables output mode hints.";
    }
    else if (button == Key::Service)
    {
        gStringForHint = LANG_RU ?
            "Кнопка СЕРВИС открывает меню сервисных возможностей."
            :
            "СЕРВИС button opens a menu of service options.";
    }
    else if (button == Key::Start)
    {
        gStringForHint = LANG_RU ?
            "Кнопка ПУСК/СTOП запускает и останавливает процесс сбора информации."
            :
            "ПУСК/СTOП button starts and stops the process of gathering information.";
    }
    else if (button == Key::ChannelA)
    {
        gStringForHint = LANG_RU ?
            "1. Кнопка КАНАЛ1 открывает меню настроек канала 1.\n"
            "2. Нажатие и удержание кнопки КАНАЛ1 в течение 0.5с устанавливает смещение канала 1 по вертикали 0В."
            :
            "1. КАНАЛ1 button opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the button КАНАЛ1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (button == Key::ChannelB)
    {
        gStringForHint = LANG_RU ?
            "1. Кнопка КАНАЛ2 открывает меню настроек канала 2.\n"
            "2. Нажатие и удержание кнопки КАНАЛ2 в течение 0.5с устанавливает смещение канала 2 по вертикали 0В."
            :
            "1. КАНАЛ2 button opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the button КАНАЛ2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (button == Key::Time)
    {
        gStringForHint = LANG_RU ?
            "1. Кнопка РАЗВ открывает меню настроек развертки.\n"
            "2. Нажатие и удержание кнопки РАЗВ в течение 0.5с устанавливает смещение по горизонтали 0с."
            :
            "1. РАЗВ button open the settings menu sweep.\n"
            "2. Pressing and holding the button РАЗВ for 0.5s Is the offset horizontal 0s.";
    }
    else if (button == Key::Synchronization)
    {
        gStringForHint = LANG_RU ?
            "1. Кнопка СИНХР открывает меню настроек синхронизации.\n"
            "2. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\" производит автоматическую настройку уровня синхронизации.\n"
            "3. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Сброс уровня\" устанавливает уровень синхронизации 0В."
            :
            "1. СИНХР button opens a menu settings synchronization.\n"
#ifndef WIN32
#pragma push
#pragma diag_suppress 192
#endif
            "2. Pressing and holding the button СИНХР for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99\x41utolevel\" automatically adjust the trigger level.\n"
#ifndef WIN32
#pragma pop
#endif
            "3. Pressing and holding the button СИНХР for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig level\" sets the trigger level 0V.";
    }
    else
    {
        shortPressureButton = button;
    }
}


void Menu::PressButton(Key::E button)
{
    Sound::ButtonPress();
    if (SHOW_HELP_HINTS)
    {
        ProcessButtonForHint(button);
        return;
    }

    if (!MenuIsShown())
    {
        for (int i = SIZE_BUFFER_FOR_BUTTONS - 1; i > 0; i--)
        {
            bufferForButtons[i] = bufferForButtons[i - 1];
        }
        bufferForButtons[0] = button;
      
        if (std::memcmp(bufferForButtons, sampleBufferForButtons, SIZE_BUFFER_FOR_BUTTONS * sizeof(Key::E)) == 0)
        {
            SHOW_DEBUG_MENU = 1;
            Display::ShowWarningGood(Warning::MenuDebugEnabled);
        }
    }
    pressButton = button;
};


void Menu::ReleaseButton(Key::E button)
{
    Sound::ButtonRelease();
    if (SHOW_HELP_HINTS == 0)
    {
        releaseButton = button;
    }
};


void Menu::RotateRegSetRight(void)
{   
    if (SHOW_HELP_HINTS == 0)
    {
        angleRegSet++;
        Display::Redraw();
    }
};


void Menu::RotateRegSetLeft(void)
{
    if (SHOW_HELP_HINTS == 0)
    {
        angleRegSet--;
        Display::Redraw();
    }
};


void* Menu::ItemUnderKey(void)
{
    return itemUnderKey;
};


void Menu::SetAutoHide(bool)
{
    if(!MenuIsShown())
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


char* Menu::StringNavigation(char buffer[100])
{
    if(SHOW_STRING_NAVIGATION && MenuIsShown())
    {
        buffer[0] = 0;
        const char *titles[10] = {0};
        int numTitle = 0;
        Item *item = OpenedItem();
        if(IsMainPage(item))
        {
            return 0;
        }
        while(!IsMainPage(item))
        {
            titles[numTitle++] = TitleItem(item);
            item = item->Keeper();
        }
        for(int i = 9; i >= 0; i--)
        {
            if(titles[i])
            {
                std::strcat(buffer, titles[i]);
                if(i != 0)
                {
                    std::strcat(buffer, " - ");
                }
            }
        }
        return buffer;
    }
    return 0;
}


void Menu::OnTimerAutoHide()
{
    ShowMenu(false);
    Timer::Disable(TypeTimer::MenuAutoHide);
}


void Menu::ProcessingShortPressureButton()
{
    if(shortPressureButton != Key::None)
    {
        if (shortPressureButton == Key::Memory && MODE_BTN_MEMORY_IS_SAVE && FLASH_DRIVE_IS_CONNECTED == 1)
        {
            EXIT_FROM_SET_NAME_TO = MenuIsShown() ? RETURN_TO_MAIN_MENU : RETURN_TO_DISABLE_MENU;
            Memory_SaveSignalToFlashDrive();
            shortPressureButton = Key::None;
            return;
        }
        Display::Redraw();
        Menu::SetAutoHide(true);

        Key::E button = shortPressureButton;

        do //-V2530
        {
            if(button == Key::Menu)                                   // Если нажата кнопка МЕНЮ и мы не находимся в режме настройки измерений.
            {
                if(!MenuIsShown())
                {
                    ShowMenu(true);
                }
                else
                {
                    CloseOpenedItem();
                }
            }
            else if (MenuIsShown() && IsFunctionalButton(button))       // Если меню показано и нажата функциональная клавиша
            {
                Item *item = ItemUnderButton(button);
                if (SHOW_HELP_HINTS)
                {
                    SetItemForHint(item);
                }
                else
                {
                    item->ShortPress();
                }
            }
            else                                                        // Если меню не показано.
            {
                NamePage::E name = ((const Page *)OpenedItem())->GetName();
                if(button == Key::ChannelA && name == NamePage::Channel0)
                {
                    SET_ENABLED_A = !sChannel_Enabled(Channel::A);
                    OnChanged_InputA(true);
                    break;
                }
                if(button == Key::ChannelB && name == NamePage::Channel1)
                {
                    SET_ENABLED_B = !sChannel_Enabled(Channel::B);
                    OnChanged_InputB(true);
                    break;
                }

                const Item *page = PageForButton(button);
                if(page)
                {
                    page->SetCurrent(true);
                    page->Open(true);
                    ShowMenu(true);
                }
            }
        } while(false);

        shortPressureButton = Key::None;
    }
}


void Menu::ProcessingLongPressureButton(void)
{
    if(longPressureButton != Key::None)
    {
        Display::Redraw();
        Menu::SetAutoHide(true);

        if(longPressureButton == Key::Time)
        {
            FPGA::SetTShift(0);
        }
        else if(longPressureButton == Key::Synchronization)
        {
            FPGA::SetTrigLev(TRIG_SOURCE, TrigLevZero);
        }
        else if(longPressureButton == Key::ChannelA)
        {
            FPGA::SetRShift(Channel::A, RShiftZero);
        }
        else if(longPressureButton == Key::ChannelB)
        {
            FPGA::SetRShift(Channel::B, RShiftZero);
        }
        else if(longPressureButton == Key::Menu)
        {
                ShowMenu(!MenuIsShown());
        }
        else if(MenuIsShown() && IsFunctionalButton(longPressureButton))
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


void Menu::ProcessingRegulatorSet(void)
{
    if (angleRegSet == 0)
    {
        return;
    }

    if (MenuIsShown() || OpenedItem()->Type() != TypeItem::Page)
    {
        Item *item = CurrentItem();
        TypeItem::E type = item->Type();
        if (OpenedItem()->Type() == TypeItem::Page && (type == TypeItem::ChoiceReg || type == TypeItem::Governor || type == TypeItem::IP || type == TypeItem::MAC))
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
            item = OpenedItem();
            type = item->Type();
            if (MenuIsMinimize())
            {
                CurrentPageSBregSet(angleRegSet);
            }
            else if (type == TypeItem::Page || type == TypeItem::IP || type == TypeItem::MAC || type == TypeItem::Choice || type == TypeItem::ChoiceReg || type == TypeItem::Governor)
            {
                if (ChangeOpenedItem(item, angleRegSet))
                {
                    angleRegSet = 0;
                }
                return;
            }
            else if (type == TypeItem::GovernorColor)
            {
                item->Change(angleRegSet);
            }
            else if (type == TypeItem::Time)
            {
                angleRegSet > 0 ? ((TimeItem *)item)->IncCurrentPosition() : ((TimeItem *)item)->DecCurrentPosition();
            }
        }
    }

    angleRegSet = 0;
}


void Menu::ProcessingPressButton(void)
{
    if (pressButton == Key::Start && !MODE_WORK_IS_LATEST)
    {
        FPGA::OnPressStartStop();
    } 
    else if((pressButton >= Key::F1 && pressButton <= Key::F5) || pressButton == Key::Menu)
    {
        if (pressButton != Key::Menu)
        {
            itemUnderKey = ItemUnderButton(pressButton);
        }
    }
    pressButton = Key::None;
}


void Menu::ProcessingReleaseButton(void)
{
    if(releaseButton >= Key::F1 && releaseButton <= Key::F5 || pressButton == Key::Menu)
    {
        itemUnderKey = 0;
        releaseButton = Key::None;
    }
}


void Menu::ChangeStateFlashDrive()
{
    if(FLASH_DRIVE_IS_CONNECTED == 0)
    {
        if(GetNameOpenedPage() == NamePage::SB_FileManager)
        {
            ((Page *)OpenedItem())->ShortPressOnItem(0);
        }
    }
    else if(FLASH_AUTOCONNECT)
    {
        NEED_OPEN_FILE_MANAGER = 1;
    }
}


extern const Page pService;


void Menu::OpenItemTime(void)
{
    Display::ShowWarningGood(Warning::TimeNotSet);
    Menu::ShortPressureButton(Key::Service);
    Menu::UpdateInput();
    Display::Update();
    for (int i = 0; i < 2; i++)
    {
        Menu::RotateRegSetRight();
        Menu::UpdateInput();
        Display::Update();
    }
    Menu::ShortPressureButton(Key::F4);
    Menu::UpdateInput();
    Display::Update();
}


bool Menu::NeedForFireSetLED()    // Возвращает true, если лампочка УСТАНОВКА должна гореть
{
    if (!MenuIsShown())
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
    
    TypeItem::E typeCurrentItem = CurrentItem()->Type();
    if (typeCurrentItem == TypeItem::Governor    ||
        typeCurrentItem == TypeItem::ChoiceReg)
    {
        return true;
    }

    TypeItem::E typeOpenedItem = OpenedItem()->Type();
    if (typeOpenedItem == TypeItem::Choice       ||
        (typeOpenedItem == TypeItem::Page && ((Page *)OpenedItem())->NumSubPages() > 1)
        )
    {
        return true;
    }
    
    return false;
}


void Menu::SwitchSetLED(void)
{
    static bool first = true;
    static bool prevState = false;  // true - горит, false - не горит

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
        ShortPressureButton(Key::Menu);
        UpdateInput();
        Display::Update(false);
    }

    if (!MenuIsShown())
    {
        ShortPressureButton(Key::Menu);
        UpdateInput();
        Display::Update(false);
    }

    for (int i = 0; i < 5 * stepAngleRegSet + 1; i++)
    {
        RotateRegSetLeft();
        UpdateInput();
        Display::Update(false);
    }

    angleRegSet = 0;

    for (int i = 0; i < 2 * stepAngleRegSet + 1; i++)
    {
        RotateRegSetRight();
        UpdateInput();
        Display::Update(false);
    }

    angleRegSet = 0;

    ShortPressureButton(Key::F2);
    UpdateInput();
    Display::Update(false);

    ShortPressureButton(Key::F4);
    UpdateInput();
    Display::Update(false);

    for (int i = 0; i < stepAngleRegSet + 1; i++)
    {
        RotateRegSetLeft();
        UpdateInput();
        Display::Update(false);
    }

    for (int i = 0; i < 2; i++)
    {
        ShortPressureButton(Key::F1);
        UpdateInput();
        Display::Update(false);
    }
}
