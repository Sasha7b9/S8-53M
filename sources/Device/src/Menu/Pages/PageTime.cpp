#include "defines.h"
#include "common/Utils/Math_c.h"
#include "FPGA/FPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageMemory.h"
#include "Settings/Settings.h"


extern const Page pTime;


extern const Choice mcSample;           // ��������� - �������
static bool  IsActive_Sample();
extern const Choice mcPeakDet;          // ��������� - ��� ���
static bool  IsActive_PeakDet();
void        OnChanged_PeakDet(bool active);
extern const Choice mcTPos;             // ��������� - To
void        OnChanged_TPos(bool active);
extern const Choice mcSelfRecorder;     // ��������� - ���������
static bool  IsActive_SelfRecorder();
extern const Choice mcDivRole;          // ��������� - �-��� ��/���


extern const Page mainPage;


// ��������� ///////////////////
DEF_PAGE_5(pTime, mainPage, NamePage::Time,
    "���������", "SCAN",
    "�������� ��������� ��������.",
    "Contains scan settings.",
    mcSample,
    mcPeakDet,
    mcTPos,
    mcSelfRecorder,
    mcDivRole,
    nullptr, nullptr, nullptr, nullptr
)

// ��������� - ������� -------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcSample, &pTime,
    "�������", "Sampling",
    "\"��������\" - \n"
    "\"�������������\" -"
    ,
    "\"Real\" - \n"
    "\"Equals\" - "
    ,
    "�������� �����", "Real",
    "�������������",  "Equals",
    SAMPLE_TYPE, IsActive_Sample, nullptr, nullptr
)


static bool IsActive_Sample(void)
{
    return sTime_RandomizeModeEnabled();
}

// ��������� - ��� ��� -------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcPeakDet, &pTime,
    "��� ���", "Pic deat",
    "��������/��������� ������� ��������.",
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
            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // ������-�� ��� ������� ��������� �������� ����������. ��� ��� � ������������.
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

// ��������� - To ------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcTPos, &pTime,
    "\x7b", "\x7b",
    "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����.",
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "����",  "Left",
    "�����", "Center",
    "�����", "Right",
    SET_TPOS, nullptr, OnChanged_TPos, nullptr
)

void OnChanged_TPos(bool active)
{
    ChangeC_Memory_NumPoints(active);
    FPGA::SetTShift(TSHIFT);
}

// ��������� - ��������� -----------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcSelfRecorder, &pTime,
    "���������", "Self-Recorder",
    "��������/��������� ����� ���������. ���� ����� �������� �� ��������� 20��/��� � ����� ���������.",
    "Turn on/off the recorder. This mode is available for scanning 20ms/div and slower.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_SELFRECORDER, IsActive_SelfRecorder, nullptr, nullptr
)

static bool IsActive_SelfRecorder(void)
{
    return SET_TBASE >= MIN_TBASE_P2P;
}

// ��������� - �-��� ��/��� --------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcDivRole, &pTime,
    "�-��� ��/���", "Func Time/DIV"
    ,
    "����� ������� ��� ����� �����/���: � ������ ����� ���������� (����/���� � ��������� ����):\n"
    "1. \"�����\" - ��������� �������� �� �������.\n"
    "2. \"������\" - ����������� �� ������."
    ,
    "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
    "1. \"Time\" - change the time shift.\n"
    "2. \"Memory\" - moving from memory."
    ,
    "�����",  "Time",
    "������", "Memory",
    TIME_DIV_XPOS, nullptr, nullptr, nullptr
)
