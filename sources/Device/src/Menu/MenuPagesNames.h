#pragma once

// ����� ID ���� �������������� � ���� �������
enum NamePage
{
    Page::EDisplay,               // ������
    Page::EDisplayAccumulation,   // ������� - ����������
    Page::EDisplayAverage,        // ������� - ����������
    Page::EDisplayGrid,           // ������� - �����
    Page::EDisplayRefresh,        // ������� - ����������
    Page::EMainPage,              // ������� �������� ����. �� �� ������ ��� ���������
    Page::EDebug,                 // �������
    Page::EDebugConsole,          // ������� - �������
    Page::EDebugADC,              // ������� - ���
    Page::EDebugADCbalance,       // ������� - ��� - ������
    Page::EDebugADCstretch,       // ������� - ��� - ��������
    Page::EDebugADCrShift,        // ������� - ��� - ���. ��������
    Page::EDebugADCaltShift,      // ������� - ��� - ���. ����. ���.
    Page::EDebugShowRegisters,    // ������� - ��������
    Page::EDebugRandomizer,       // ������� - ����-���
    Page::EDebugInformation,      // ������� - ����������
    Page::EDebugSound,            // ������� - ����
    Page::EDebugAltera,           // ������� - �������
    Page::EChannel0,              // ����� 1
    Page::EChannel1,              // ����� 2
    Page::ECursors,               // �������
    Page::ETrig,                  // �����.
    Page::EMemory,                // ������
    Page::EMemoryExt,             // ������ - �����. ��
    Page::EMeasures,              // ���������
    Page::EMeasuresField,         // ��������� - ����
    Page::EService,               // ������
    Page::EServiceDisplay,        // ������ - �������
    Page::EServiceDisplayColors,  // ������ - ������� - �����
    Page::EServiceCalibrator,     // ������ - ����������
    Page::EServiceEthernet,       // ������ - ETHERNET
    Page::EHelp,                  // ������
    Page::ETime,                  // ���������
    Page::EMath,                  // ������ - ����������
    Page::EMathFFT,
    Page::ETrigAuto,              // ����� - ��� �����
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
    Page::ENoPage
};             // ��� ���������� ������� ����� �������� ������������� SettingsMenu � SSettings.c
