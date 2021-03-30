// 2021/03/30 14:02:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Console
{
public:

    static void Draw();

    static void AddStringToIndicating(pchar string);

    static void SetPauseForConsole(bool pause);

    static void OneStringUp();

    static void OneStringDown();

private:

    static int FirstEmptyString();

    static int CalculateFreeSize();

    static void DeleteFirstString();

    static void AddString(pchar string);
};
