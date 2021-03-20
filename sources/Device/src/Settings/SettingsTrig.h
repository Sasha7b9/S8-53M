#pragma once
#include "common/Settings/SettingsTypes_.h"


#define TRIG_MODE_FIND          (set.trig.modeFind)                     // SettingsTrig.modeFind
#define TRIG_MODE_FIND_IS_AUTO  (TRIG_MODE_FIND == TrigModeFind::Auto)  // \c true, если автоматическая установка синхронизации.
#define TRIG_MODE_FIND_IS_HAND  (TRIG_MODE_FIND == TrigModeFind::Hand)  // \c true, если синхронизация устанавливается вручную.


// Настройки синхронизации
struct SettingsTrig
{
    StartMode::E         startMode;          // Режим запуска.
    TrigSource::E        source;             // Источник.
    TrigPolarity::E      polarity;           // Тип синхронизации.
    TrigInput::E         input;              // Вход синхронизации.
    int16                levelRel[3];        // Уровень синхронизации для трёх источников.
    ModeLongPressTrig::E modeLongPressTrig;  // Режим работы длительного нажатия кнопки СИНХР.
    TrigModeFind::E      modeFind;           // Поиск синхронизации - вручную или автоматически.
};
