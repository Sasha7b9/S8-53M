#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"

static bool IsActive_Sample()
{
    return FPGA::SET::InRandomizeMode();
}

DEF_CHOICE_2(mcSample, PageTime::self,
    "�������", "Sampling",
    "\"��������\" - \n"
    "\"�������������\" -"
    ,
    "\"Real\" - \n"
    "\"Equals\" - "
    ,
    "�������� �����", "Real",
    "�������������", "Equals",
    set.time.sample_type, IsActive_Sample, nullptr, nullptr
)

static bool IsActive_PeakDet()
{
    return (TBase::Get() >= TBase::MIN_PEAK_DET);
}

void PageTime::OnChanged_PeakDet(bool active)
{
    if (active)
    {
        PeackDetMode::Set(PeackDetMode::Get());
        TBase::Set(TBase::Get());
        if (!PeackDetMode::IsEnabled())
        {
            volatile int8 shift[2][3] =
            {
                {0, set.chan[ChA].balance_shift_ADC, (int8)set.debug.balance_ADC[ChA]},
                {0, set.chan[ChB].balance_shift_ADC, (int8)set.debug.balance_ADC[ChB]}
            };

//            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shift[0][BALANCE_ADC_TYPE], false);
//            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shift[1][BALANCE_ADC_TYPE], false);
        }
        else
        {
//            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // ������-�� ��� ������� ��������� �������� ����������. ��� ��� � ������������.
//            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, 3, false);
        }
        if (!PeackDetMode::IsEnabled())
        {
            int centerX = TShift::GetInMemory() + Grid::Width() / 2;
            TShift::SetInMemory((int16)(centerX * 2 - Grid::Width() / 2));
            set.memory.enum_points_fpga = set.time.oldNumPoints;
            ChangeC_Memory_NumPoints(true);
        }
        else if (PeackDetMode::IsEnabled())
        {
            int centerX = TShift::GetInMemory() + Grid::Width() / 2;
            LIMITATION(set.display.shift_in_memory, (int16)(centerX / 2 - Grid::Width() / 2), 0,
                (int16)(SettingsMemory::GetNumPoints(false) - Grid::Width()));
            ChangeC_Memory_NumPoints(true);
        }
    }
    else
    {
        Display::ShowWarningBad(Warning::TooSmallSweepForPeakDet);
    }
}

DEF_CHOICE_2(mcPeakDet, PageTime::self,
    "��� ���", "Pic deat",
    "��������/��������� ������� ��������.",
    "Turns on/off peak detector.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.time.peak_det, IsActive_PeakDet, PageTime::OnChanged_PeakDet, nullptr
)

void PageTime::OnChanged_TPos(bool active)
{
    ChangeC_Memory_NumPoints(active);
    TShift::Set(TShift::Get());
}

DEF_CHOICE_3(mcTPos, PageTime::self,
    "\x7b", "\x7b",
    "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����.",
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "����", "Left",
    "�����", "Center",
    "�����", "Right",
    set.time.t_pos, nullptr, PageTime::OnChanged_TPos, nullptr
)

static bool IsActive_SelfRecorder()
{
    return (FPGA::SET::InModeP2P());
}

DEF_CHOICE_2(mcSelfRecorder, PageTime::self,
    "���������", "Self-Recorder",
    "��������/��������� ����� ���������. ���� ����� �������� �� ��������� 20��/��� � ����� ���������.",
    "Turn on/off the recorder. This mode is available for scanning 20ms/div and slower.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.time.self_recorder, IsActive_SelfRecorder, nullptr, nullptr
)

DEF_CHOICE_2(mcDivRole, PageTime::self,
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
    "�����", "Time",
    "������", "Memory",
    set.time.function_time, nullptr, nullptr, nullptr
)

DEF_PAGE_5(pageTime, PageMain::self, NamePage::Time,
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

const Page *PageTime::self = &pageTime;
