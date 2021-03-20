#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


void PageTrig::OnPress_Mode(bool)
{
    FPGA::Stop(false);
    if (!StartMode::IsSingle())
    {
        FPGA::Start();
    }
}

DEF_CHOICE_3(mcMode, PageTrig::self,
    "Режим", "Mode"
    ,
    "Задаёт режим запуска:\n"
    "1. \"Авто\" - запуск происходит автоматически.\n"
    "2. \"Ждущий\" - запуск происходит по уровню синхронизации.\n"
    "3. \"Однократный\" - запуск происходит по достижении уровня синхронизации один раз. Для следующего измерения нужно нажать кнопку ПУСК/СТОП."
    ,
    "Sets the trigger mode:\n"
    "1. \"Auto\" - start automatically.\n"
    "2. \"Standby\" - the launch takes place at the level of synchronization.\n"
    "3. \"Single\" - the launch takes place on reaching the trigger levelonce. For the next measurement is necessary to press the START/STOP."
    ,
    "Авто ", "Auto",
    "Ждущий", "Wait",
    "Однократный", "Single",
    set.trig.startMode, nullptr, PageTrig::OnPress_Mode, nullptr
)

static void OnChanged_Source(bool)
{
    TrigSource::Set(TrigSource::Get());
}

DEF_CHOICE_3(mcSource, PageTrig::self,
    "Источник", "Source",
    "Выбор источника сигнала синхронизации.",
    "Synchronization signal source choice.",
    "Канал 1", "Channel 1",
    "Канал 2", "Channel 2",
    "Внешний", "External",
    set.trig.source, nullptr, OnChanged_Source, nullptr
)

static void OnChanged_Polarity(bool)
{
    TrigPolarity::Set(TrigPolarity::Get());
}

DEF_CHOICE_2(mcPolarity, PageTrig::self,
    "Полярность", "Polarity"
    ,
    "1. \"Фронт\" - запуск происходит по фронту синхроимпульса.\n"
    "2. \"Срез\" - запуск происходит по срезу синхроимпульса."
    ,
    "1. \"Front\" - start happens on the front of clock pulse.\n"
    "2. \"Back\" - start happens on a clock pulse cut."
    ,
    "Фронт", "Front",
    "Срез", "Back",
    set.trig.polarity, nullptr, OnChanged_Polarity, nullptr
)

static void OnChanged_Input(bool)
{
    TrigInput::Set(TrigInput::Get());
}

DEF_CHOICE_4(mcInput, PageTrig::self,
    "Вход", "Input"
    ,
    "Выбор связи с источником синхронизации:\n"
    "1. \"ПС\" - полный сигнал.\n"
    "2. \"АС\" - закрытый вход.\n"
    "3. \"ФНЧ\" - фильтр низких частот.\n"
    "4. \"ФВЧ\" - фильтр высоких частот."
    ,
    "The choice of communication with the source of synchronization:\n"
    "1. \"SS\" - a full signal.\n"
    "2. \"AS\" - a gated entrance.\n"
    "3. \"LPF\" - low-pass filter.\n"
    "4. \"HPF\" - high-pass filter frequency."
    ,
    "ПС", "Full",
    "АС", "AC",
    "ФНЧ", "LPF",
    "ФВЧ", "HPF",
    set.trig.input, nullptr, OnChanged_Input, nullptr
)

DEF_CHOICE_2(mcAutoFind_Mode, PageTrig::PageAutoFind::self,
    "Режим", "Mode"
    ,
    "Выбор режима автоматического поиска синхронизации:\n"
    "1. \"Ручной\" - поиск производится по нажатию кнопки \"Найти\" или по удержанию в течение 0.5с кнопки СИНХР, если установлено \"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\".\n"
    "2. \"Автоматический\" - поиск производится автоматически."
    ,
    "Selecting the automatic search of synchronization:\n"
    "1. \"Hand\" - search is run on pressing of the button \"Find\" or on deduction during 0.5s the СИНХР button if it is established \"SERVICE\x99Mode long СИНХР\x99\x41utolevel\".\n"
    "2. \"Auto\" - the search is automatically."
    ,
    "Ручной", "Hand",
    "Автоматический", "Auto",
    set.trig.modeFind, nullptr, nullptr, nullptr
)

static bool IsActive_AutoFind_Search()
{
    return TrigModeFind::IsHand();
}

static void OnPress_AutoFind_Search()
{
    TrigLev::FindAndSet();
}

DEF_BUTTON(mbAutoFind_Search, PageTrig::PageAutoFind::self,
    "Найти", "Search",
    "Производит поиск уровня синхронизации.",
    "Runs for search synchronization level.",
    IsActive_AutoFind_Search, OnPress_AutoFind_Search
)

DEF_PAGE_2(pageAutoFind, PageTrig::self, NamePage::TrigAuto,
    "ПОИСК", "SEARCH",
    "Управление автоматическим поиском уровня синхронизации.",
    "Office of the automatic search the trigger level.",
    mcAutoFind_Mode,
    mbAutoFind_Search,
    nullptr, nullptr, nullptr, nullptr
)

DEF_PAGE_5(pageTrig, PageMain::self, NamePage::Trig,
    "СИНХР", "TRIG",
    "Содержит настройки синхронизации.",
    "Contains synchronization settings.",
    mcMode,
    mcSource,
    mcPolarity,
    mcInput,
    *PageTrig::PageAutoFind::self,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageTrig::self = &pageTrig;
const Page *PageTrig::PageAutoFind::self = &pageAutoFind;
