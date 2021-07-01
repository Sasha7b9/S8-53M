// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Device
{
public:
    static void Init();
    static void Update();
    static bool needResetSettings;  // Если 1, то необходим сброс настроек

private:

    static void WriteFPS();
};
