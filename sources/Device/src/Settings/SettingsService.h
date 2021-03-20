#pragma once


#define IP_ADDRESS          (set.service.IPaddress)             // SettingsService.IPaddress

// Настройки меню СЕРВИС.
struct SettingsService
{ //-V802
    bool              screenWelcomeEnable; // Будет ли показываться экран приглашения при включении прибора. \todo убрать этот артефакт.
    bool              soundEnabled;        // Включены ли звуки.
    CalibratorMode::E calibrator;          // Режим работы калибратора.
    int8              IPaddress;           // IP-адрес (временно).
    ColorScheme::E    colorScheme;         // Цветовая схеама.
};

bool sService_MathEnabled();

float sService_MathGetFFTmaxDBabs();
