#pragma once

// ����� ID ���� �������������� � ���� �������
enum NamePage
{
    NamePage::Display,               // ������
    NamePage::DisplayAccumulation,   // ������� - ����������
    NamePage::DisplayAverage,        // ������� - ����������
    NamePage::DisplayGrid,           // ������� - �����
    NamePage::DisplayRefresh,        // ������� - ����������
    NamePage::MainPage,              // ������� �������� ����. �� �� ������ ��� ���������
    NamePage::Debug,                 // �������
    NamePage::DebugConsole,          // ������� - �������
    NamePage::DebugADC,              // ������� - ���
    NamePage::DebugADCbalance,       // ������� - ��� - ������
    NamePage::DebugADCstretch,       // ������� - ��� - ��������
    NamePage::DebugADCrShift,        // ������� - ��� - ���. ��������
    NamePage::DebugADCaltShift,      // ������� - ��� - ���. ����. ���.
    NamePage::DebugShowRegisters,    // ������� - ��������
    NamePage::DebugRandomizer,       // ������� - ����-���
    NamePage::DebugInformation,      // ������� - ����������
    NamePage::DebugSound,            // ������� - ����
    NamePage::DebugAltera,           // ������� - �������
    NamePage::Channel0,              // ����� 1
    NamePage::Channel1,              // ����� 2
    NamePage::Cursors,               // �������
    NamePage::Trig,                  // �����.
    NamePage::Memory,                // ������
    NamePage::MemoryExt,             // ������ - �����. ��
    NamePage::Measures,              // ���������
    NamePage::MeasuresField,         // ��������� - ����
    NamePage::Service,               // ������
    NamePage::ServiceDisplay,        // ������ - �������
    NamePage::ServiceDisplayColors,  // ������ - ������� - �����
    NamePage::ServiceCalibrator,     // ������ - ����������
    NamePage::ServiceEthernet,       // ������ - ETHERNET
    NamePage::Help,                  // ������
    NamePage::Time,                  // ���������
    NamePage::Math,                  // ������ - ����������
    NamePage::MathFFT,
    NamePage::TrigAuto,              // ����� - ��� �����
    NamePage::SB_Curs,               // ������� �������� ������� - ���������
    NamePage::SB_MeasTuneMeas,       // ������� �������� ��������� - ���������
    NamePage::SB_MeasTuneField,      // ������� �������� ��������� - ���� - ������
    NamePage::SB_MemLatest,          // ������� �������� ������ - ���������
    NamePage::SB_MemInt,             // ������� �������� ������ - ����� ��
    NamePage::SB_MemExtSetMask,      // ������� �������� ������ - ����� �� - �����
    NamePage::SB_MemExtSetName,      // ���������� ����� ����������� ����� �� ������ ��� ������� ����� ����� ��� ��������������� ����� ����� �� - ��� �����
    NamePage::SB_FileManager,        // ������� �������� ������ - ����� �� - �����
    NamePage::SB_Help,               // ������� �������� HELP
    NamePage::SB_MathCursorsFFT,     // ������� �������� ������ - ���������� - �������
    NamePage::SB_MathFunction,       // ������ - ���������� - �������
    NamePage::SB_Information,        // ������ - ����������
    NamePage::SB_SerialNumber,       // ������ - �/�
    NamePage::Count,
    NamePage::NoPage
};             // ��� ���������� ������� ����� �������� ������������� SettingsMenu � SSettings.c
