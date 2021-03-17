// 2021/03/17 14:42:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


    // Поиск сигнала
struct AutoFinderFPGA
{
    static void Find();

    static bool FindWave(Channel::E chan);

    static Range::E AccurateFindRange(Channel::E chan);

    static TBase::E FindTBase(Channel::E chan);

    static TBase::E AccurateFindTBase(Channel::E chan);

    static bool auto_find_in_progress;

private:

    static uint8 CalculateMinWithout0(const uint8 buffer[100]);

    static uint8 CalculateMaxWithout255(const uint8 buffer[100]);
};
