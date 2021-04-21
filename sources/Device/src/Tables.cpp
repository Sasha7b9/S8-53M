// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Tables.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Settings/Settings.h"


// ������� �������� �������� � ������������ ��� ����� ������
pchar Tables::symbolsAlphaBet[0x48] =
{
    /* 0x00 */ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", /* 0x19 */
    /* 0x1a */ " ", "_", "-", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", /* 0x26 */
    /* 0x27 */ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", /* 0x40 */
    //         0x41  0x42  0x43  0x44  0x45  0x46   0x47   - ��� ���������� ����� �������� � ���� �������
    /* 0x41 */ "%y", "%m", "%d", "%H", "%M", "%S", "%N" /* 0x47 */
    //         0x01  0x02  0x03  0x04  0x05  0x06   0x07   - ��� ����� ���������� �������� �������� � set.memory.fileNameMask
};


const char* Tables::GetWarning(Warning::E warning)
{
    static pchar warnings[Warning::Count][2] =
    {
        {"������ ����� 1 - �����/���",      "LIMIT CHANNEL 1 - VOLTS/DIV"},
        {"������ ����� 2 - �����/���",      "LIMIT CHANNEL 2 - VOLTS/DIV"},
        {"������ ��������� - �����/���",    "LIMIT SWEEP - TIME/DIV"},
        {"������� ������� ��������",        "SET_ENABLED PEAK. DET."},
        {"������ ����� 1 - \x0d",           "LIMIT CHANNEL 1 - \x0d"},
        {"������ ����� 2 - \x0d",           "LIMIT CHANNEL 2 - \x0d"},
        {"������ ��������� - �������",      "LIMIT SWEEP - LEVEL"},
        {"������ ��������� - \x97",         "LIMIT SWEEP - \x97"},
        {"���. ���. �� �������� �� ���������� ����� 0.5���/���", "PEAK. DET. NOT WORK ON SWEETS LESS THAN 0.5us/div"},
        {"��������� �� ����� �������� �� ������� ���¨�����", "SELF-RECORDER DOES NOT WORK FAST SCANS"},
        {"���� ��������",                   "FILE IS SAVED"},
        {"������ ��������",                 "SIGNAL IS SAVED"},
        {"������ ������",                   "SIGNAL IS DELETED"},
        {"���� ������� ��������",           "MENU DEBUG IS SET_ENABLED"},
        {"��������� ��������� ������, ���� ����������� ������", "WAIT ChA FEW SECONDS, GOES ChA MOVEMENT DATA"},
        {"����� �� �����������. ������ ���������� ��� ������", "TIME IS NOT SET. YOU CAN INSTALL IT NOW"},
        {"������ �� ������",                "SIGNAL IS NOT FOUND"},
        {"�� ���������� ��������� 10��/��� ���������� �������-", "AT SCANNER SLOWLY 10ms/div DESIRABLY SET \"SCAN - \x7b\" IN"},
        {"������ \"��������� - \x7b\" � ��������� \"����\" ��� ������-", "SWITCH \"Left\" FOR TO ACCELERATE THE OUTPUT SIGNAL"},
        {"��� ������ �������",              ""},
        {"����� ���� ���������",            "VERY SMALL MEASURES"},
        {"��� ���������� ��������� � ����", "FOR THE INTRODUCTION OF CHANGES"},
        {"��������� ������",                "IN FORCE SWITCH OFF THE DEVICE"},
        {"��������� ���������� ���",        "DISCONNECT CALCULATION OF FFT"},
        {"��������� �������������� �������", "DISCONNECT MATHEMATICAL FUNCTION"},
        {"�������� ���������",              "FIRMWARE SAVED"},
        {"������ OTP ��������� ��������",   "OPT memory fully completed"}
    };
    return warnings[warning][LANG];
}


void Tables::DrawStr(int index, int x, int y)
{
    pchar str = symbolsAlphaBet[index];
    if (index == set.memory.index_cur_symbol_name_mask)
    {
        Region(Font::GetLengthText(str), 9).Fill(x - 1, y, Color::FLASH_10);
    }
    Text(symbolsAlphaBet[index]).Draw(x, y, (index == set.memory.index_cur_symbol_name_mask) ?
        Color::FLASH_01 : Color::FILL);
}
