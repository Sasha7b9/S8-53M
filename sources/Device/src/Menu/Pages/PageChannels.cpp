#include "defines.h"
#include "common/Log_c.h"
#include "Settings/SettingsTypes.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include "FPGA/FPGA.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


extern const char chanInputRu[] =   "1. \"Вкл\" - выводить сигнал на экран.\n"
                                    "2. \"Откл\" - не выводить сигнал на экран.";
extern const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

extern const char chanCoupleRu[] =  "Задаёт вид связи с источником сигнала.\n"
                                    "1. \"Пост\" - открытый вход.\n"
                                    "2. \"Перем\" - закрытый вход.\n"
                                    "3. \"Земля\" - вход соединён с землёй.";
extern const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

extern const char chanFiltrRu[] = "Включает/отключает фильтр для ограничения полосы пропускания.";
extern const char chanFiltrEn[] = "Includes/switches-off the filter for restriction of a pass-band.";

extern const char chanInverseRu[] = "При \"Вкл\" сигнал на экране будет симметрично отражён относительно U = 0В.";
extern const char chanInverseEn[] = "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

extern const char chanMultiplierRu[] = "Ослабление сигнала:\n\"x1\" - сигнал не ослабляется.\n\"x10\" - сигнал ослабляется в 10 раз";
extern const char chanMultiplierEn[] = "Attenuation: \n\"x1\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";



extern const Page mainPage;

void PageChannelA::OnChanged_Input(bool)
{
    Panel::EnableLEDChannel0(sChannel_Enabled(Channel::A));
}

DEF_CHOICE_2(mcInputA, PageChannelA::self,
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_ENABLED_A, nullptr, PageChannelA::OnChanged_Input, nullptr
)

static void OnChanged_CoupleA(bool)
{
    FPGA::SetModeCouple(Channel::A, SET_COUPLE_A);
}

DEF_CHOICE_3(mcCoupleA, PageChannelA::self,
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост", "AC",
    "Перем", "DC",
    "Земля", "Ground",
    SET_COUPLE_A, nullptr, OnChanged_CoupleA, nullptr
)

static void OnChanged_FiltrA(bool)
{
    FPGA::EnableChannelFiltr(Channel::A, SET_FILTR_A);
}

DEF_CHOICE_2(mcFiltrA, PageChannelA::self,
    "Фильтр", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_FILTR_A, nullptr, OnChanged_FiltrA, nullptr
)

static void OnChanged_InverseA(bool)
{
    FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
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
    SET_DIVIDER(Channel::A), nullptr, nullptr, nullptr
)

DEF_PAGE_5(pChanA, mainPage, NamePage::Channel0,
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

const Page *PageChannelA::self = &pChanA;

void PageChannelB::OnChanged_Input(bool)
{
    Panel::EnableLEDChannel1(sChannel_Enabled(Channel::B));
}

DEF_CHOICE_2(mcInputB, PageChannelB::self,
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_ENABLED_B, nullptr, PageChannelB::OnChanged_Input, nullptr
)

static void OnChanged_CoupleB(bool)
{
    FPGA::SetModeCouple(Channel::B, SET_COUPLE_B);
}

DEF_CHOICE_3(mcCoupleB, PageChannelB::self,
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост", "AC",
    "Перем", "DC",
    "Земля", "Ground",
    SET_COUPLE_B, nullptr, OnChanged_CoupleB, nullptr
)

static void OnChanged_FiltrB(bool)
{
    FPGA::EnableChannelFiltr(Channel::B, SET_FILTR_B);
}

DEF_CHOICE_2(mcFiltrB, PageChannelB::self,
    "Фильтр", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_FILTR_B, nullptr, OnChanged_FiltrB, nullptr
)

static void OnChanged_InverseB(bool)
{
    FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
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
    SET_DIVIDER(Channel::B), nullptr, nullptr, nullptr
)

DEF_PAGE_5(pChanB, mainPage, NamePage::Channel1,
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

const Page *PageChannelB::self = &pChanB;
