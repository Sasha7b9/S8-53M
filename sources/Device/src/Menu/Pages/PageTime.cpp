#include "defines.h"
#include "common/Utils/Math_c.h"
#include "FPGA/FPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageMemory.h"
#include "Settings/Settings.h"


extern const Page pTime;


extern const Choice mcSample;           // РАЗВЕРТКА - Выборка
static bool  IsActive_Sample();
extern const Choice mcPeakDet;          // РАЗВЕРТКА - Пик дет
static bool  IsActive_PeakDet();
void        OnChanged_PeakDet(bool active);
extern const Choice mcTPos;             // РАЗВЕРТКА - To
void        OnChanged_TPos(bool active);
extern const Choice mcSelfRecorder;     // РАЗВЕРТКА - Самописец
static bool  IsActive_SelfRecorder();
extern const Choice mcDivRole;          // РАЗВЕРТКА - Ф-ция ВР/ДЕЛ


extern const Page mainPage;


// РАЗВЕРТКА ///////////////////
DEF_PAGE_5(pTime, mainPage, NamePage::Time,
    "РАЗВЕРТКА", "SCAN",
    "Содержит настройки развёртки.",
    "Contains scan settings.",
    mcSample,
    mcPeakDet,
    mcTPos,
    mcSelfRecorder,
    mcDivRole,
    nullptr, nullptr, nullptr, nullptr
)

// РАЗВЕРТКА - Выборка -------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcSample, &pTime,
    "Выборка", "Sampling",
    "\"Реальная\" - \n"
    "\"Эквивалентная\" -"
    ,
    "\"Real\" - \n"
    "\"Equals\" - "
    ,
    "Реальное время", "Real",
    "Эквивалентная",  "Equals",
    SAMPLE_TYPE, IsActive_Sample, nullptr, nullptr
)


static bool IsActive_Sample(void)
{
    return sTime_RandomizeModeEnabled();
}

// РАЗВЕРТКА - Пик дет -------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcPeakDet, &pTime,
    "Пик дет", "Pic deat",
    "Включает/выключает пиковый детектор.",
    "Turns on/off peak detector.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    PEAKDET, IsActive_PeakDet, OnChanged_PeakDet, nullptr
)

static bool IsActive_PeakDet(void)
{
    return (SET_TBASE >= MIN_TBASE_PEC_DEAT);
}

void OnChanged_PeakDet(bool active)
{
    if (active)
    {
        FPGA::SetPeackDetMode(PEAKDET);
        FPGA::SetTBase(SET_TBASE);
        if (PEAKDET_IS_DISABLE)
        {
            int8 shift[2][3] =
            {
                {0, SET_BALANCE_ADC_A, static_cast<int8>(BALANCE_ADC_A)},
                {0, SET_BALANCE_ADC_B, static_cast<int8>(BALANCE_ADC_B)}
            };

            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shift[0][BALANCE_ADC_TYPE], false);
            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shift[1][BALANCE_ADC_TYPE], false);
        }
        else
        {
            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // Почему-то при пиковом детекторе смещение появляется. Вот его и компенсируем.
            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, 3, false);
        }
        if (PEAKDET_IS_DISABLE)
        {
            int centerX = SHIFT_IN_MEMORY + Grid::Width() / 2;
            SHIFT_IN_MEMORY = static_cast<int16>(centerX * 2 - Grid::Width() / 2);
            ENUM_POINTS = set.time.oldNumPoints;
            ChangeC_Memory_NumPoints(true);
        }
        else if (PEAKDET_IS_ENABLE)
        {
            int centerX = SHIFT_IN_MEMORY + Grid::Width() / 2;
            LIMITATION(SHIFT_IN_MEMORY, static_cast<int16>(centerX / 2 - Grid::Width() / 2), 0, static_cast<int16>(sMemory_GetNumPoints(false) - Grid::Width()));
            ChangeC_Memory_NumPoints(true);
        }
    }
    else
    {
        Display::ShowWarningBad(Warning::TooSmallSweepForPeakDet);
    }
}

// РАЗВЕРТКА - To ------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcTPos, &pTime,
    "\x7b", "\x7b",
    "Задаёт точку привязки нулевого смещения по времени к экрану - левый край, центр, правый край.",
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "Лево",  "Left",
    "Центр", "Center",
    "Право", "Right",
    SET_TPOS, nullptr, OnChanged_TPos, nullptr
)

void OnChanged_TPos(bool active)
{
    ChangeC_Memory_NumPoints(active);
    FPGA::SetTShift(TSHIFT);
}

// РАЗВЕРТКА - Самописец -----------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcSelfRecorder, &pTime,
    "Самописец", "Self-Recorder",
    "Включает/выключает режим самописца. Этот режим доступен на развёртках 20мс/дел и более медленных.",
    "Turn on/off the recorder. This mode is available for scanning 20ms/div and slower.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_SELFRECORDER, IsActive_SelfRecorder, nullptr, nullptr
)

static bool IsActive_SelfRecorder(void)
{
    return SET_TBASE >= MIN_TBASE_P2P;
}

// РАЗВЕРТКА - Ф-ция ВР/ДЕЛ --------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcDivRole, &pTime,
    "Ф-ция ВР/ДЕЛ", "Func Time/DIV"
    ,
    "Задаёт функцию для ручки ВРЕМЯ/ДЕЛ: в режиме сбора информации (ПУСК/СТОП в положении ПУСК):\n"
    "1. \"Время\" - изменение смещения по времени.\n"
    "2. \"Память\" - перемещение по памяти."
    ,
    "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
    "1. \"Time\" - change the time shift.\n"
    "2. \"Memory\" - moving from memory."
    ,
    "Время",  "Time",
    "Память", "Memory",
    TIME_DIV_XPOS, nullptr, nullptr, nullptr
)
