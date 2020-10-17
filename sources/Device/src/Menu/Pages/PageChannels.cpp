#include "defines.h"
#include "common/Log_c.h"
#include "Settings/SettingsTypes.h"
#include "Menu/MenuItems.h"
#include "Panel/Panel.h"
#include "FPGA/FPGA.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


extern const Page pChanA;
extern const Page pChanB;



extern const Choice mcInputA;                   // ����� 1 - ����
void OnChanged_InputA(bool active);
extern const Choice mcCoupleA;                  // ����� 1 - �����
void OnChanged_CoupleA(bool active);
extern const Choice mcFiltrA;                   // ����� 1 - ������
void OnChanged_FiltrA(bool active);
extern const Choice mcInverseA;                 // ����� 1 - ��������
static void OnChanged_InverseA(bool active);
extern const Choice mcMultiplierA;              // ����� 1 - ���������

extern const Choice mcInputB;                   // ����� 2 - ����
void OnChanged_InputB(bool active);
extern const Choice mcCoupleB;                  // ����� 2 - �����
void OnChanged_CoupleB(bool active);
extern const Choice mcFiltrB;                   // ����� 2 - ������
void OnChanged_FiltrB(bool active);
extern const Choice mcInverseB;                 // ����� 2 - ��������
static void OnChanged_InverseB(bool active);
extern const Choice mcMultiplierB;              // ����� 2 - ���������



extern const char chanInputRu[] =   "1. \"���\" - �������� ������ �� �����.\n"
                                    "2. \"����\" - �� �������� ������ �� �����.";
extern const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

extern const char chanCoupleRu[] =  "����� ��� ����� � ���������� �������.\n"
                                    "1. \"����\" - �������� ����.\n"
                                    "2. \"�����\" - �������� ����.\n"
                                    "3. \"�����\" - ���� ������� � �����.";
extern const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

extern const char chanFiltrRu[] = "��������/��������� ������ ��� ����������� ������ �����������.";
extern const char chanFiltrEn[] = "Includes/switches-off the filter for restriction of a pass-band.";

extern const char chanInverseRu[] = "��� \"���\" ������ �� ������ ����� ����������� ������ ������������ U = 0�.";
extern const char chanInverseEn[] = "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

extern const char chanMultiplierRu[] = "���������� �������:\n\"x1\" - ������ �� �����������.\n\"x10\" - ������ ����������� � 10 ���";
extern const char chanMultiplierEn[] = "Attenuation: \n\"x1\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";



extern const Page mainPage;


// ����� 1 /////////////////////////
DEF_PAGE_5(pChanA, mainPage, NamePage::Channel0,
    "����� 1", "CHANNEL 1",
    "�������� ��������� ������ 1.",
    "Contains settings of the channel 1.",
    mcInputA,
    mcCoupleA,
    mcFiltrA,
    mcInverseA,
    mcMultiplierA,
    nullptr, nullptr, nullptr, nullptr
)


// ����� 1 - ���� ------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcInputA, pChanA,
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    (int8)SET_ENABLED_A, nullptr, OnChanged_InputA, nullptr
)


void OnChanged_InputA(bool)
{
    Panel::EnableLEDChannel0(sChannel_Enabled(Channel::A));
}


// ����� 1 - ����� -----------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcCoupleA, pChanA,
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����",  "AC",
    "�����", "DC",
    "�����", "Ground",
    (int8)SET_COUPLE_A, nullptr, OnChanged_CoupleA, nullptr
)


void OnChanged_CoupleA(bool)
{
    FPGA::SetModeCouple(Channel::A, SET_COUPLE_A);
}


// ����� 1 - ������ ----------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcFiltrA, pChanA,
    "������", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    (int8)SET_FILTR_A, nullptr, OnChanged_FiltrA, nullptr
)


void OnChanged_FiltrA(bool)
{
    FPGA::EnableChannelFiltr(Channel::A, SET_FILTR_A);
}


// ����� 1 - �������� --------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcInverseA, pChanA,
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    (int8)SET_INVERSE_A, nullptr, OnChanged_InverseA, nullptr
)


static void OnChanged_InverseA(bool)
{
    FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
}


// ����� 1 - ��������� -------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcMultiplierA, pChanA,
    "���������", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "�1",  "x1",
    "x10", "x10",
    (int8)SET_DIVIDER(Channel::A), nullptr, nullptr, nullptr
)


// ����� 2 /////////////////////////
DEF_PAGE_5(pChanB, mainPage, NamePage::Channel1,
    "����� 2", "CHANNEL 2",
    "�������� ��������� ������ 2.",
    "Contains settings of the channel 2.",
    mcInputB,
    mcCoupleB,
    mcFiltrB,
    mcInverseB,
    mcMultiplierB,
    nullptr, nullptr, nullptr, nullptr
)


// ����� 2 - ���� ------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcInputB, pChanB,
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    (int8)SET_ENABLED_B, nullptr, OnChanged_InputB, nullptr
)

void OnChanged_InputB(bool)
{
    Panel::EnableLEDChannel1(sChannel_Enabled(Channel::B));
}


// ����� 2 - ����� -----------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcCoupleB, pChanB,
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����",  "AC",
    "�����", "DC",
    "�����", "Ground",
    (int8)SET_COUPLE_B, nullptr, OnChanged_CoupleB, nullptr
)

void OnChanged_CoupleB(bool)
{
    FPGA::SetModeCouple(Channel::B, SET_COUPLE_B);
}

// ����� 2 - ������ ----------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcFiltrB, pChanB,
    "������", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    (int8)SET_FILTR_B, nullptr, OnChanged_FiltrB, nullptr
)

void OnChanged_FiltrB(bool)
{
    FPGA::EnableChannelFiltr(Channel::B, SET_FILTR_B);
}


// ����� 2 - �������� --------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcInverseB, pChanB,
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    (int8)SET_INVERSE_B, nullptr, OnChanged_InverseB, nullptr
)

static void OnChanged_InverseB(bool)
{
    FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
}


// ����� 2 - ��������� -------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcMultiplierB, pChanB,
    "���������", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "�1",  "x1",
    "x10", "x10",
    (int8)SET_DIVIDER(Channel::B), nullptr, nullptr, nullptr
)
