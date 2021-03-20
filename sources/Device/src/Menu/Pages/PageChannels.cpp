#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"


static const char chanInputRu[] =   "1. \"Вкл\" - выводить сигнал на экран.\n"
                                    "2. \"Откл\" - не выводить сигнал на экран.";
static const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

static const char chanCoupleRu[] =  "Задаёт вид связи с источником сигнала.\n"
                                    "1. \"Пост\" - открытый вход.\n"
                                    "2. \"Перем\" - закрытый вход.\n"
                                    "3. \"Земля\" - вход соединён с землёй.";
static const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

static const char chanFiltrRu[] = "Включает/отключает фильтр для ограничения полосы пропускания.";
static const char chanFiltrEn[] = "Includes/switches-off the filter for restriction of a pass-band.";

static const char chanInverseRu[] = "При \"Вкл\" сигнал на экране будет симметрично отражён относительно U = 0В.";
static const char chanInverseEn[] = "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

static const char chanMultiplierRu[] = "Ослабление сигнала:\n\"x1\" - сигнал не ослабляется.\n\"x10\" - сигнал ослабляется в 10 раз";
static const char chanMultiplierEn[] = "Attenuation: \n\"x1\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";



void PageChannelA::OnChanged_Input(bool)
{
    Panel::EnableLEDChannel0(ChA.IsEnabled());
}

DEF_CHOICE_2(mcInputA, PageChannelA::self,
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_ENABLED_A, nullptr, PageChannelA::OnChanged_Input, nullptr
)

void PageChannelA::OnChanged_Couple(bool)
{
    ModeCouple::Set(ChA, SET_COUPLE_A);
}

DEF_CHOICE_3(mcCoupleA, PageChannelA::self,
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост", "AC",
    "Перем", "DC",
    "Земля", "Ground",
    SET_COUPLE_A, nullptr, PageChannelA::OnChanged_Couple, nullptr
)

void PageChannelA::OnChanged_Filtr(bool)
{
    ChannelFiltr::Enable(ChA, SET_FILTR_A);
}

DEF_CHOICE_2(mcFiltrA, PageChannelA::self,
    "Фильтр", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_FILTR_A, nullptr, PageChannelA::OnChanged_Filtr, nullptr
)

static void OnChanged_InverseA(bool)
{
    RShift::Set(ChA, RShift::Get(ChA));
}

DEF_CHOICE_2(mcInverseA, PageChannelA::self,
    "Инверсия", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_INVERSE_A, nullptr, OnChanged_InverseA, nullptr
)

DEF_CHOICE_2(mcMultiplierA, PageChannelA::self,
    "Множитель", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "х1", "x1",
    "x10", "x10",
    SET_DIVIDER(ChA), nullptr, nullptr, nullptr
)

DEF_PAGE_5(pageChanA, PageMain::self, NamePage::ChannelA,
    "КАНАЛ 1", "CHANNEL 1",
    "Содержит настройки канала 1.",
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
    Panel::EnableLEDChannel1(ChB.IsEnabled());
}

DEF_CHOICE_2(mcInputB, PageChannelB::self,
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_ENABLED_B, nullptr, PageChannelB::OnChanged_Input, nullptr
)

void PageChannelB::OnChanged_Couple(bool)
{
    ModeCouple::Set(ChB, SET_COUPLE_B);
}

DEF_CHOICE_3(mcCoupleB, PageChannelB::self,
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост", "AC",
    "Перем", "DC",
    "Земля", "Ground",
    SET_COUPLE_B, nullptr, PageChannelB::OnChanged_Couple, nullptr
)

void PageChannelB::OnChanged_Filtr(bool)
{
    ChannelFiltr::Enable(ChB, SET_FILTR_B);
}

DEF_CHOICE_2(mcFiltrB, PageChannelB::self,
    "Фильтр", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_FILTR_B, nullptr, PageChannelB::OnChanged_Filtr, nullptr
)

static void OnChanged_InverseB(bool)
{
    RShift::Set(ChB, RShift::Get(ChB));
}

DEF_CHOICE_2(mcInverseB, PageChannelB::self,
    "Инверсия", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_INVERSE_B, nullptr, OnChanged_InverseB, nullptr
)

DEF_CHOICE_2(mcMultiplierB, PageChannelB::self,
    "Множитель", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "х1", "x1",
    "x10", "x10",
    SET_DIVIDER(ChB), nullptr, nullptr, nullptr
)

DEF_PAGE_5(pageChanB, PageMain::self, NamePage::ChannelB,
    "КАНАЛ 2", "CHANNEL 2",
    "Содержит настройки канала 2.",
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
