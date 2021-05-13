// 2021/05/12 10:18:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"


struct SettingsNRST
{
    struct SettingsRangomizer
    {
        int16   num_ave;                // �� �������� ���������� ��������� ������
        int16   num_smooth;             // ����� ����� ��� ����������� �������
        int16   num_measures_for_gates; // ����� ��������� ��� �����
        int16   shift_T0;               // �������������� �������� �� ������� ��� ������ ��������
        bool    show_info;              // �������� ������ � ��������� ��������
    };

    struct SettingsADC
    {
        BalanceADCtype::E type_balance; // ��� ������������
        int16             balance[2];   // �������� ��������������� �������� ��� ��� ������ ������������
        StretchADCtype::E type_stretch; // ��� �������� ������.
        int16             stretch[2];   // �������� �������� ������ ��� ������� ������.
    };

    struct SettingsConsole
    {
        int16   num_srings;     // ����� ����� � �������.
        int8    size_font;      // ������ ������ ������� - 0 - 5, 1 - 8,
        bool    in_pause;       // ������� ����, ��� ������� ��������� � ������ �����. ����� ����� ��������, ��� ����� 
                                // ��������� ��� �� ���������� � �� ���������.
    };

    struct SettingsRegisters
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

    bool               show_stats;                 // ���������� ���������� �� ������ (fps, ��������).
    SettingsRangomizer rand;
    SettingsADC        adc;
    SettingsConsole    console;
    SettingsRegisters  show_registers;

    static int  GetSizeFontForConsole();        // ���������� ������ ������, ������� ����� �������� ��������� � �������.
};


extern SettingsNRST setNRST;
