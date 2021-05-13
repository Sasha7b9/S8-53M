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
        StretchADCtype::E type_stretch; // ��� �������� ������.
    };

    struct SettingsChannel
    {
        int16   balance_hand;                // �������� ��������������� �������� ��� ��� ������ ������������
        int8    balance_auto;                // �������������� �������� ���, ������������ � �������� ����������
        int16   stretch_hand;                // �������� �������� ������ ��� ������� ������
        float   stretch_auto;                // ��������, ������������ � �������� ����������
        int16   rshift_add[Range::Count][2]; // ���������� �������� ��� ��������� (0) � ��������� (1) ������
    };

    struct SettingsConsole
    {
        int16   num_srings;     // ����� ����� � �������.
        int8    size_font;      // ������ ������ ������� - 0 - 5, 1 - 8,
        bool    in_pause;       // ������� ����, ��� ������� ��������� � ������ �����. ����� ����� ��������, ��� ����� 
                                // ��������� ��� �� ���������� � �� ���������.

        // ���������� ������ ������, ������� ����� �������� ��������� � �������.
        static int  GetSizeFontForConsole();
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
    SettingsChannel    chan[Channel::Count];
    SettingsConsole    console;
    SettingsRegisters  show_registers;

    // ��� ������� ����� �������� ����� ������� ��������� �������������� ���������
    static void CommonOnChanged();

    static void Save();
};


extern SettingsNRST setNRST;
