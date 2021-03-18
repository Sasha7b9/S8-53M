// 2021/03/17 14:42:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


    // Поиск сигнала
struct AutoFinderFPGA
{
    static void Find();

    static bool FindWave(Channel::E ch);

    static Range::E AccurateFindRange(const Channel &ch);

    static TBase::E FindTBase(Channel::E ch);

    static TBase::E AccurateFindTBase(Channel::E ch);

    static bool auto_find_in_progress;

private:

    static uint8 CalculateMinWithout0(const uint8 buffer[100]);

    static uint8 CalculateMaxWithout255(const uint8 buffer[100]);
};
