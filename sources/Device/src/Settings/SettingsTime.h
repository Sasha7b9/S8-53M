#pragma once


#define TIME_DIV_XPOS                       (set.time.timeDivXPos)
#define TIME_DIV_XPOS_IS_SHIFT_IN_MEMORY    (TIME_DIV_XPOS == FunctionTime::ShiftInMemory)


// ��������� ��� X.
struct SettingsTime
{ //-V802
    TBase::E            tBase;          // ������� �� �������.
    int16               tShiftRel;      // �������� �� �������.
    FunctionTime::E     timeDivXPos;
    TPos::E             tPos;           // �������� ������������� � ������.
    SampleType::E       sampleType;     // ��� ������� ��� ������ �������������.
    PeackDetMode::E     peakDet;        // ����� ������ �������� ���������
    bool                selfRecorder;   // ������� �� ����� ���������.
    ENUM_POINTS_FPGA::E oldNumPoints;   // \brief ����� ������������� � ����� �������� ���������, �������������
                                        // ���������� ����� � 1024, � ����  ���������� ��, ��� ����, ����� �����
                                        // ������������.
};
