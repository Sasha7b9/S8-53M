// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class SU
{
public:

    static char ToUpper(char symbol);

    static char ToLower(char symbol);

    static char *DoubleToString(double value);

    static bool StringToDouble(double *value, pchar buffer);

    static bool IsDigit(char symbol);
};
