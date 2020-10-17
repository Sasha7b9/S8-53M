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



extern const Choice mcInputA;                   // КАНАЛ 1 - Вход
void OnChanged_InputA(bool active);
extern const Choice mcCoupleA;                  // КАНАЛ 1 - Связь
void OnChanged_CoupleA(bool active);
extern const Choice mcFiltrA;                   // КАНАЛ 1 - Фильтр
void OnChanged_FiltrA(bool active);
extern const Choice mcInverseA;                 // КАНАЛ 1 - Инверсия
static void OnChanged_InverseA(bool active);
extern const Choice mcMultiplierA;              // КАНАЛ 1 - Множитель

extern const Choice mcInputB;                   // КАНАЛ 2 - Вход
void OnChanged_InputB(bool active);
extern const Choice mcCoupleB;                  // КАНАЛ 2 - Связь
void OnChanged_CoupleB(bool active);
extern const Choice mcFiltrB;                   // КАНАЛ 2 - Фильтр
void OnChanged_FiltrB(bool active);
extern const Choice mcInverseB;                 // КАНАЛ 2 - Инверсия
static void OnChanged_InverseB(bool active);
extern const Choice mcMultiplierB;              // КАНАЛ 2 - Множитель



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


// КАНАЛ 1 /////////////////////////
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


// КАНАЛ 1 - Вход ------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcInputA, pChanA,
    "Вход", "Input",
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


// КАНАЛ 1 - Связь -----------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcCoupleA, pChanA,
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост",  "AC",
    "Перем", "DC",
    "Земля", "Ground",
    (int8)SET_COUPLE_A, nullptr, OnChanged_CoupleA, nullptr
)


void OnChanged_CoupleA(bool)
{
    FPGA::SetModeCouple(Channel::A, SET_COUPLE_A);
}


// КАНАЛ 1 - Фильтр ----------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcFiltrA, pChanA,
    "Фильтр", "Filtr",
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


// КАНАЛ 1 - Инверсия --------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcInverseA, pChanA,
    "Инверсия", "Inverse",
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


// КАНАЛ 1 - Множитель -------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcMultiplierA, pChanA,
    "Множитель", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "х1",  "x1",
    "x10", "x10",
    (int8)SET_DIVIDER(Channel::A), nullptr, nullptr, nullptr
)


// КАНАЛ 2 /////////////////////////
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


// КАНАЛ 2 - Вход ------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcInputB, pChanB,
    "Вход", "Input",
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


// КАНАЛ 2 - Связь -----------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcCoupleB, pChanB,
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост",  "AC",
    "Перем", "DC",
    "Земля", "Ground",
    (int8)SET_COUPLE_B, nullptr, OnChanged_CoupleB, nullptr
)

void OnChanged_CoupleB(bool)
{
    FPGA::SetModeCouple(Channel::B, SET_COUPLE_B);
}

// КАНАЛ 2 - Фильтр ----------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcFiltrB, pChanB,
    "Фильтр", "Filtr",
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


// КАНАЛ 2 - Инверсия --------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcInverseB, pChanB,
    "Инверсия", "Inverse",
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


// КАНАЛ 2 - Множитель -------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcMultiplierB, pChanB,
    "Множитель", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "х1",  "x1",
    "x10", "x10",
    (int8)SET_DIVIDER(Channel::B), nullptr, nullptr, nullptr
)
