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
        int16   gate_min;
        int16   gate_max;
    };

    struct SettingsADC
    {
        StretchADCtype::E type_stretch; // ��� �������� ������.
    };

    struct SettingsChannel
    {
        int16 balance_ADC;                      // ������ ���
        int16 rshift_hand[3];                   // ���������� ��������, ��������������� � ������ ������ ���
                                                // ModeCouple::DC � ��� ����� �������������� ����������
        int16 rshift_auto[Range::Count][2];     // ���������� ��������, ������� ������� ���� ��� ����������
                                                // � ������������. ��� z == 0 - ModeCouple::DC, ��� z == 1
                                                // - ModeCouple::AC
        int16 stretch_hand;                     // �������� �������� ������ ��� ������� ������
        float stretch_auto;                     // ��������, ������������ � �������� ����������
        int16 rshift_add[Range::Count][2];      // ���������� �������� ��� ��������� (0) � ��������� (1) ������

        // ��������� �������������� �������� � �������� � ����
        void StoreAndResetRShifts(int16 shifts[3]);

        // ������������ ����������� �������������� �������� (��, ������� ��������������� ������� ��� ��������������
        // ����������
        void RestoreRShifts(int16 shifts[3]);
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

    uint               size;                // ������ ���������.�� ���� ��� �������� �� ������ ����� ����������, ���
                                            // ��������� ����� - ����� ����� �������� 0xFFFFFFFF
    uint               crc32;               // ����������� ����� ��� �������� ������������ ���������� ������
    uint               number;
    bool               show_stats;          // ���������� ���������� �� ������ (fps, ��������).
    SettingsRangomizer rand;
    SettingsADC        adc;
    SettingsChannel    chan[Channel::Count];
    SettingsConsole    console;
    SettingsRegisters  show_registers;
    CalibrationMode::E calibration_mode[Channel::Count];
    uint               unused_crc32;        // �������������� ����������. ����� ��� ������� crc32. ������ ����� ����

    // ������������� ��� ���������. ��������� ���������� ������� �������� � ROM � �� �������� � ������, ���� ���������
    // ����. ���� �������� ���� - ������������� ���������� �� ���������
    void Load();

    // ���������� �������� � ROM. ���������� ������ ���� ��������� � setNRST �� ��������� � ����������� � ROM
    void Save();

    void Test();

    // ��� ������� ����� �������� ����� ������� ��������� �������������� ���������
    static void CommonOnChanged();
    static void CommonOnChanged(bool);

    bool operator!=(const SettingsNRST &rhs);
};


extern SettingsNRST setNRST;
