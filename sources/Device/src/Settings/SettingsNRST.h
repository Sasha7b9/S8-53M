// 2021/05/12 10:18:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"


struct OutputRegisters
{
    bool launch;
    bool flag;           // �������� �� ���� ����������.
    bool rShiftA;
    bool rShiftB;
    bool trigLev;
    bool rangeA;
    bool rangeB;
    bool chanParamA;
    bool chanParamB;
    bool trigParam;
    bool tShift;
    bool tBase;
};


struct SettingsNRST
{
    int16             num_srings;                 // ����� ����� � �������.
    int8              size_font;                  // ������ ������ ������� - 0 - 5, 1 - 8,
    bool              console_in_pause;           // \brief ������� ����, ��� ������� ��������� � ������ �����. �����
                                                  // ����� ��������, ��� ����� 
                                                  // ��������� ��� �� ���������� � �� ���������.
    BalanceADCtype::E balance_ADC_type;           // ��� ������������.
    int16             balance_ADC[2];             // �������� ��������������� �������� ��� ��� ������ ������������.
    StretchADCtype::E stretch_ADC_type;           // ��� �������� ������.
    int16             stretch_ADC[2];             // �������� �������� ������ ��� ������� ������.
    int16             num_measures_for_gates;     // ����� ��������� ��� �����.
    int16             shift_T0;                   // �������������� �������� �� ������� ��� ������ �������� ������
                                                  // �������������.
    bool              show_stats;                 // ���������� ���������� �� ������ (fps, ��������).
    int16             num_ave_for_rand;           // �� �������� ���������� ��������� ������ � ������ �������������.
    bool              viewAlteraWrittingData;     // ���������� �� ������, ������ � �������.
    bool              viewAllAlteraWrittingData;  // \brief ���������� �� ��� ������, ������ � ������� (���� false, ��
                                                  // ��������� ������ ������� ����� 
                                                  // START, STOP �� ������������).
    int16             altShift;                   // ���������� �������� ��� ���������� ������� �������������� ������
                                                  // �������������.
    int16             num_smooth_for_rand;        // ����� ����� ��� ����������� ������� � �������������
    OutputRegisters   show_registers;

    static int  GetSizeFontForConsole();        // ���������� ������ ������, ������� ����� �������� ��������� � �������.
};


extern SettingsNRST setNRST;
