#pragma once


// �������������� ������.
enum PanelButton
{
    B_Empty,        // ������ �� ������
    B_Channel0,     // ����� 1
    B_Service,      // ������
    B_Channel1,     // ����� 2
    B_Display,      // �������
    B_Time,         // ����
    B_Memory,       // ������
    B_Trig,         // �����
    B_Start,        // ����/�����
    B_Cursors,      // �������
    B_Measures,     // �����
    B_Power,        // �������
    B_Help,         // ������
    B_Menu,         // ����
    B_F1,           // 1
    B_F2,           // 2
    B_F3,           // 3
    B_F4,           // 4
    B_F5,           // 5
    B_NumButtons            // ����� ���������� ������
};

// �������������� �����������.
enum Regulator
{
    R_Empty     = 0,    // ��������� �� ��������
    R_Range0    = 20,   // �����/��� ���. 1
    R_RShift0   = 21,   
    R_Range1    = 22,   // �����/��� ���. 2
    R_RShift1   = 23,
    R_TBase     = 24,   // �����/���
    R_TShift    = 25,
    R_TrigLev   = 26,   // �������
    R_Set       = 27    // ���������
};

// ����������� �������� �����������.
enum RegulatorDirection
{
    Direction_Left,     // �������� ���������� �����
    Direction_Right,    // �������� ���������� ������
    Direction_None      // �������� �����������
};


#define NUM_REGULATORS (R_Set - R_Range0 + 1)


const char* NameButton(PanelButton button);
