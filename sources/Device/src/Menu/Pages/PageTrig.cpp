#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


extern const Page pTrig;



extern const Choice mcMode;                     // ����� - �����
void          OnPress_Mode(bool active);
extern const Choice mcSource;                   // ����� - ��������
static void OnChanged_Source(bool active);
extern const Choice mcPolarity;                 // ����� - ����������
static void OnChanged_Polarity(bool active);
extern const Choice mcInput;                    // ����� - ����
static void OnChanged_Input(bool active);
extern const Page   mpAutoFind;                 // ����� - �����
extern const Choice mcAutoFind_Mode;            // ����� - ����� - �����
extern const Button mbAutoFind_Search;          // ����� - ����� - �����
static bool  IsActive_AutoFind_Search();
static void   OnPress_AutoFind_Search();

// ����� ///////////////////////////
DEF_PAGE_5(pTrig, PageMain::self, NamePage::Trig,
    "�����", "TRIG",
    "�������� ��������� �������������.",
    "Contains synchronization settings.",
    mcMode,
    mcSource,
    mcPolarity,
    mcInput,
    mpAutoFind,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageTrig::self = &pTrig;


// ����� - ����� -------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcMode, &pTrig,
    "�����", "Mode"
    ,
    "����� ����� �������:\n"
    "1. \"����\" - ������ ���������� �������������.\n"
    "2. \"������\" - ������ ���������� �� ������ �������������.\n"
    "3. \"�����������\" - ������ ���������� �� ���������� ������ ������������� ���� ���. ��� ���������� ��������� ����� ������ ������ ����/����."
    ,
    "Sets the trigger mode:\n"
    "1. \"Auto\" - start automatically.\n"
    "2. \"Standby\" - the launch takes place at the level of synchronization.\n"
    "3. \"Single\" - the launch takes place on reaching the trigger levelonce. For the next measurement is necessary to press the START/STOP."
    ,
    "���� ",       "Auto",
    "������",      "Wait",
    "�����������", "Single",
    START_MODE, nullptr, OnPress_Mode, nullptr
)

void OnPress_Mode(bool)
{
    FPGA::Stop(false);
    if (!START_MODE_IS_SINGLE)
    {
        FPGA::Start();
    }
}


// ����� - �������� ----------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcSource, &pTrig,
    "��������", "Source",
    "����� ��������� ������� �������������.",
    "Synchronization signal source choice.",
    "����� 1", "Channel 1",
    "����� 2", "Channel 2",
    "�������", "External",
    TRIG_SOURCE, nullptr, OnChanged_Source, nullptr
)

static void OnChanged_Source(bool)
{
    FPGA::SetTrigSource(TRIG_SOURCE);
}


// ����� - ���������� --------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcPolarity, &pTrig,
    "����������", "Polarity"
    ,
    "1. \"�����\" - ������ ���������� �� ������ ��������������.\n"
    "2. \"����\" - ������ ���������� �� ����� ��������������."
    ,
    "1. \"Front\" - start happens on the front of clock pulse.\n"
    "2. \"Back\" - start happens on a clock pulse cut."
    ,
    "�����", "Front",
    "����",  "Back",
    TRIG_POLARITY, nullptr, OnChanged_Polarity, nullptr
)

static void OnChanged_Polarity(bool)
{
    FPGA::SetTrigPolarity(TRIG_POLARITY);
}


// ����� - ���� --------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(mcInput, &pTrig,
    "����", "Input"
    ,
    "����� ����� � ���������� �������������:\n"
    "1. \"��\" - ������ ������.\n"
    "2. \"��\" - �������� ����.\n"
    "3. \"���\" - ������ ������ ������.\n"
    "4. \"���\" - ������ ������� ������."
    ,
    "The choice of communication with the source of synchronization:\n"
    "1. \"SS\" - a full signal.\n"
    "2. \"AS\" - a gated entrance.\n"
    "3. \"LPF\" - low-pass filter.\n"
    "4. \"HPF\" - high-pass filter frequency."
    ,
    "��",  "Full",
    "��",  "AC",
    "���", "LPF",
    "���", "HPF",
    TRIG_INPUT, nullptr, OnChanged_Input, nullptr
)

static void OnChanged_Input(bool)
{
    FPGA::SetTrigInput(TRIG_INPUT);
}


// ����� - ����� -------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2(mpAutoFind, pTrig, NamePage::TrigAuto,
    "�����", "SEARCH",
    "���������� �������������� ������� ������ �������������.",
    "Office of the automatic search the trigger level.",
    mcAutoFind_Mode,
    mbAutoFind_Search,
    nullptr, nullptr, nullptr, nullptr
)


// ����� - ����� - ����� -----------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcAutoFind_Mode, &mpAutoFind,
    "�����", "Mode"
    ,
    "����� ������ ��������������� ������ �������������:\n"
    "1. \"������\" - ����� ������������ �� ������� ������ \"�����\" ��� �� ��������� � ������� 0.5� ������ �����, ���� ����������� \"������\x99��� ���� �����\x99�����������\".\n"
    "2. \"��������������\" - ����� ������������ �������������."
    ,
    "Selecting the automatic search of synchronization:\n"
    "1. \"Hand\" - search is run on pressing of the button \"Find\" or on deduction during 0.5s the ����� button if it is established \"SERVICE\x99Mode long �����\x99\x41utolevel\".\n"
    "2. \"Auto\" - the search is automatically."
    ,
    "������",         "Hand",
    "��������������", "Auto",
    TRIG_MODE_FIND, nullptr, nullptr, nullptr
)


// ����� - ����� - ����� -----------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(mbAutoFind_Search, mpAutoFind,
    "�����", "Search",
    "���������� ����� ������ �������������.",
    "Runs for search synchronization level.",
    IsActive_AutoFind_Search, OnPress_AutoFind_Search
)


static bool IsActive_AutoFind_Search(void)
{
    return TRIG_MODE_FIND_IS_HAND;
}

static void OnPress_AutoFind_Search(void)
{
    FPGA::FindAndSetTrigLevel();
}
