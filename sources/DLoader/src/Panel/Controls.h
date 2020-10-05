#pragma once


// �������������� ������.
struct PanelButton { enum E
{
    Empty,        // ������ �� ������
    Channel0,     // ����� 1
    Service,      // ������
    Channel1,     // ����� 2
    Display,      // �������
    Time,         // ����
    Memory,       // ������
    Trig,         // �����
    Start,        // ����/�����
    Cursors,      // �������
    Measures,     // �����
    Power,        // �������
    Help,         // ������
    Menu,         // ����
    F1,           // 1
    F2,           // 2
    F3,           // 3
    F4,           // 4
    F5,           // 5
    Count            // ����� ���������� ������
};};

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


const char* NameButton(PanelButton::E button);
