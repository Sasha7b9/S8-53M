// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"


static const char chanInputRu[] =   "1. \"���\" - �������� ������ �� �����.\n"
                                    "2. \"����\" - �� �������� ������ �� �����.";
static const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

static const char chanCoupleRu[] =  "����� ��� ����� � ���������� �������.\n"
                                    "1. \"����\" - �������� ����.\n"
                                    "2. \"�����\" - �������� ����.\n"
                                    "3. \"�����\" - ���� ������� � �����.";
static const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

static const char chanFiltrRu[] = "��������/��������� ������ ��� ����������� ������ �����������.";
static const char chanFiltrEn[] = "Includes/switches-off the filter for restriction of a pass-band.";

static const char chanInverseRu[] = "��� \"���\" ������ �� ������ ����� ����������� ������ ������������ U = 0�.";
static const char chanInverseEn[] =
                         "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

static const char chanMultiplierRu[] =
                          "���������� �������:\n\"x1\" - ������ �� �����������.\n\"x10\" - ������ ����������� � 10 ���";
static const char chanMultiplierEn[] =
                "Attenuation: \n\"x1\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";



void PageChannelA::OnChanged_Input(bool)
{
    Panel::EnableLED(TypeLED::ChanA, ChA.IsEnabled());
}

DEF_CHOICE_2(mcInputA, PageChannelA::self,
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.chan[ChA].enable, nullptr, PageChannelA::OnChanged_Input, nullptr
)

void PageChannelA::OnChanged_Couple(bool)
{
    ModeCouple::Set(ChA, set.chan[ChA].mode_couple);
}

DEF_CHOICE_3(mcCoupleA, PageChannelA::self,
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����", "AC",
    "�����", "DC",
    "�����", "Ground",
    set.chan[Channel::A].mode_couple, nullptr, PageChannelA::OnChanged_Couple, nullptr
)

void PageChannelA::OnChanged_Filtr(bool)
{
    ChannelFiltr::Enable(ChA, set.chan[ChA].filtr);
}

DEF_CHOICE_2(mcFiltrA, PageChannelA::self,
    "������", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.chan[ChA].filtr, nullptr, PageChannelA::OnChanged_Filtr, nullptr
)

static void OnChanged_InverseA(bool)
{
    RShift::Set(ChA, set.chan[ChA].rshift);
}

DEF_CHOICE_2(mcInverseA, PageChannelA::self,
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.chan[ChA].inverse, nullptr, OnChanged_InverseA, nullptr
)

DEF_CHOICE_2(mcMultiplierA, PageChannelA::self,
    "���������", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "�1", "x1",
    "x10", "x10",
    set.chan[ChA].divider, nullptr, nullptr, nullptr
)

DEF_PAGE_5(pageChanA, PageMain::self, NamePage::ChannelA,
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

void PageChannelB::OnChanged_Input(bool)
{
    Panel::EnableLED(TypeLED::ChanB, ChB.IsEnabled());
}

DEF_CHOICE_2(mcInputB, PageChannelB::self,
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.chan[ChB].enable, nullptr, PageChannelB::OnChanged_Input, nullptr
)

void PageChannelB::OnChanged_Couple(bool)
{
    ModeCouple::Set(ChB, set.chan[ChB].mode_couple);
}

DEF_CHOICE_3(mcCoupleB, PageChannelB::self,
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����", "AC",
    "�����", "DC",
    "�����", "Ground",
    set.chan[Channel::B].mode_couple, nullptr, PageChannelB::OnChanged_Couple, nullptr
)

void PageChannelB::OnChanged_Filtr(bool)
{
    ChannelFiltr::Enable(ChB, set.chan[ChB].filtr);
}

DEF_CHOICE_2(mcFiltrB, PageChannelB::self,
    "������", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.chan[ChB].filtr, nullptr, PageChannelB::OnChanged_Filtr, nullptr
)

static void OnChanged_InverseB(bool)
{
    RShift::Set(ChB, set.chan[ChB].rshift);
}

DEF_CHOICE_2(mcInverseB, PageChannelB::self,
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.chan[ChB].inverse, nullptr, OnChanged_InverseB, nullptr
)

DEF_CHOICE_2(mcMultiplierB, PageChannelB::self,
    "���������", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "�1", "x1",
    "x10", "x10",
    set.chan[ChB].divider, nullptr, nullptr, nullptr
)

DEF_PAGE_5(pageChanB, PageMain::self, NamePage::ChannelB,
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

const Page *PageChannelA::self = &pageChanA;
const Page *PageChannelB::self = &pageChanB;
