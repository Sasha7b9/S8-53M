#pragma once


#define SOUND_ENABLED       (set.service.soundEnabled)          // SettingsService.soundEnabled

#define CALIBRATOR          (set.service.calibrator)            // SettingsService.calibrator
#define CALIBRATOR_IS_FREQ  (CALIBRATOR == CalibratorMode::Freq)
#define CALIBRATOR_IS_DC    (CALIBRATOR == CalibratorMode::DC)

#define IP_ADDRESS          (set.service.IPaddress)             // SettingsService.IPaddress

#define COLOR_SCHEME                    (set.service.colorScheme)
#define IS_COLOR_SCHEME_WHITE_LETTERS   (COLOR_SCHEME == ColorScheme::WhiteLetters)


// Режим работы калибратора.
struct CalibratorMode { enum E
{
    Freq,           // На выходе калибратора 4В, 1кГц.
    DC,             // На выходе калибратора 4В постоянного напряжения.
    GND             // На выходе калибратора 0В.
};};

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
