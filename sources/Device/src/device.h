#pragma once


class Device
{
public:
    static void Init();
    static void Update();
    static bool needResetSettings;  // Если 1, то необходим сброс настроек
};
