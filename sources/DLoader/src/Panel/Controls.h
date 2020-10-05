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
struct Regulator { enum E
{
    Empty     = 0,      // ��������� �� ��������
    Range0    = 20,     // �����/��� ���. 1
    RShift0,
    Range1,             // �����/��� ���. 2
    RShift1,
    TBase,              // �����/���
    TShift,
    TrigLev,            // �������
    Set                 // ���������
};};

// ����������� �������� �����������.
enum RegulatorDirection
{
    Direction_Left,     // �������� ���������� �����
    Direction_Right,    // �������� ���������� ������
    Direction_None      // �������� �����������
};


#define NUM_REGULATORS (Regulator::Set - R_Range0 + 1)


const char* NameButton(PanelButton::E button);
