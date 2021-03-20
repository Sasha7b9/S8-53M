#pragma once


#define IP_ADDRESS          (set.service.IPaddress)             // SettingsService.IPaddress

// ��������� ���� ������.
struct SettingsService
{ //-V802
    bool              screenWelcomeEnable; // ����� �� ������������ ����� ����������� ��� ��������� �������. \todo ������ ���� ��������.
    bool              soundEnabled;        // �������� �� �����.
    CalibratorMode::E calibrator;          // ����� ������ �����������.
    int8              IPaddress;           // IP-����� (��������).
    ColorScheme::E    colorScheme;         // �������� ������.
};

bool sService_MathEnabled();

float sService_MathGetFFTmaxDBabs();
