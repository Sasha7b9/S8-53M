#include "defines.h"
#include "Tables.h"
#include "common/Log_c.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "common/Display/Font/Font_c.h"
#include "Settings/Settings.h"


using namespace Primitives;


// ��������� ��� �������� ��������� �������� �� �������.
struct TBaseStruct
{
    const char* name[2];    // �������� ��������� � ��������� ����, ��������� ��� ������ �� �����.
};


// ������ �������� �������� ��������� �� �������.
static const TBaseStruct tBases[TBase::Count] =
{
    {"2\x10��",     "2\x10ns"},
    {"5\x10��",     "5\x10ns"},
    {"10\x10��",    "10\x10ns"},
    {"20\x10��",    "20\x10ns"},
    {"50\x10��",    "50\x10ns"},
    {"0.1\x10���",  "0.1\x10us"},
    {"0.2\x10���",  "0.2\x10us"},
    {"0.5\x10���",  "0.5\x10us"},
    {"1\x10���",    "1\x10us"},
    {"2\x10���",    "2\x10us"},
    {"5\x10���",    "5\x10us"},
    {"10\x10���",   "10\x10us"},
    {"20\x10���",   "20\x10us"},
    {"50\x10���",   "50\x10us"},
    {"0.1\x10��",   "0.1\x10ms"},
    {"0.2\x10��",   "0.2\x10ms"},
    {"0.5\x10��",   "0.5\x10ms"},
    {"1\x10��",     "1\x10ms"},
    {"2\x10��",     "2\x10ms"},
    {"5\x10��",     "5\x10ms"},
    {"10\x10��",    "10\x10ms"},
    {"20\x10��",    "20\x10ms"},
    {"50\x10��",    "50\x10ms"},
    {"0.1\x10�",    "0.1\x10s"},
    {"0.2\x10�",    "0.2\x10s"},
    {"0.5\x10�",    "0.5\x10s"},
    {"1\x10�",      "1\x10s"},
    {"2\x10�",      "2\x10s"},
    {"5\x10�",      "5\x10s"},
    {"10\x10�",     "10\x10s"}
};

// ������� �������� �������� � ������������ ��� ����� ������
const char* symbolsAlphaBet[0x48] =
{
    /* 0x00 */ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", /* 0x19 */
    /* 0x1a */ " ", "_", "-", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", /* 0x26 */
    /* 0x27 */ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", /* 0x40 */
    //         0x41  0x42  0x43  0x44  0x45  0x46   0x47   - ��� ���������� ����� �������� � ���� �������
    /* 0x41 */ "%y", "%m", "%d", "%H", "%M", "%S", "%N" /* 0x47 */
    //         0x01  0x02  0x03  0x04  0x05  0x06   0x07   - ��� ����� ���������� �������� �������� � set.memory.fileNameMask
};


ENumSignalsInSec::E Tables::NumSignalsInSecToENUM(int enumSignalsInSec)
{
    if(enumSignalsInSec == 1)
    {
        return ENumSignalsInSec::_1;
    }
    else if(enumSignalsInSec == 2)
    {
        return ENumSignalsInSec::_2;
    }
    else if(enumSignalsInSec == 5)
    {
        return ENumSignalsInSec::_5;
    }
    else if(enumSignalsInSec == 10)
    {
        return ENumSignalsInSec::_10;
    }
    else if(enumSignalsInSec == 25)
    {
        return ENumSignalsInSec::_25;
    }
    LOG_ERROR("����� �������� � ������� ����� %d", enumSignalsInSec);
    return ENumSignalsInSec::_1;
}


int Tables::ENUMtoNumSignalsInSec(ENumSignalsInSec::E enumSignalsInSec)
{
    static const int fps[] = {25, 10, 5, 2, 1};
    if(enumSignalsInSec <= ENumSignalsInSec::_1)
    {
        return fps[enumSignalsInSec];
    }
    LOG_ERROR("������������ �������� %d", static_cast<int>(enumSignalsInSec));
    return 1;
}


const char* Tables::GetWarning(Warning::E warning)
{
    static const char *warnings[Warning::Count][2] =
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
        {"��������� ��������� ������, ���� ����������� ������", "WAIT Channel::A FEW SECONDS, GOES Channel::A MOVEMENT DATA"},
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


const char* Tables::GetTBaseString(TBase::E tBase)
{
    return tBases[tBase].name[LANG];
}


const char* Tables::GetTBaseStringEN(TBase::E tBase)
{
    return tBases[tBase].name[Language::English];
}


const char* Tables::RangeNameFromValue(Range::E range)
{
    static const char* names[Range::Count] =
    {
        "Range::_2mV",
        "Range::_5mV",
        "Range::_10mV",
        "Range::_20mV",
        "Range::_50mV",
        "Range::_100mV",
        "Range::_200mV",
        "Range::_500mV",
        "Range::_1V",
        "Range::_2V",
        "Range::_5V",
        "Range::_10V",
        "Range::_20V"
    };
    return names[range];
}


int Tables::GetNumChannel(Channel::E chan)
{
    return (chan == Channel::A) ? 1 : 2;
}


void Tables::DrawStr(int index, int x, int y)
{
    const char *str = symbolsAlphaBet[index];
    if (index == INDEX_SYMBOL)
    {
        Region(Font::GetLengthText(str), 9).Fill(x - 1, y, Color::FLASH_10);
    }
    Text(symbolsAlphaBet[index]).Draw(x, y, index == INDEX_SYMBOL ? Color::FLASH_01 : COLOR_FILL);
}
