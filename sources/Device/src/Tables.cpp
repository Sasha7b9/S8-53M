#include "defines.h"
#include "Tables.h"
#include "common/Log_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Display/Font/Font_.h"
#include "Settings/Settings.h"


using namespace Primitives;


// рЮАКХЖЮ ЯХЛБНКНБ ЮКТЮБХРЮ Х ЯОЕЖЯХЛБНКНБ ДКЪ ББНДЮ РЕЙЯРЮ
pchar Tables::symbolsAlphaBet[0x48] =
{
    /* 0x00 */ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", /* 0x19 */
    /* 0x1a */ " ", "_", "-", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", /* 0x26 */
    /* 0x27 */ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", /* 0x40 */
    //         0x41  0x42  0x43  0x44  0x45  0x46   0x47   - ЩРН ОНПЪДЙНБШИ МНЛЕП ЩКЕЛЕМРЮ Б ЩРНЛ ЛЮЯЯХБЕ
    /* 0x41 */ "%y", "%m", "%d", "%H", "%M", "%S", "%N" /* 0x47 */
    //         0x01  0x02  0x03  0x04  0x05  0x06   0x07   - ОНД ЩРХЛХ ГМЮВЕМХЪЛХ ЩКЕЛЕМРШ УПЮМЪРЯЪ Б set.memory.fileNameMask
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
    LOG_ERROR("вХЯКН ЯХЦМЮКНБ Б ЯЕЙСМДС ПЮБМН %d", enumSignalsInSec);
    return ENumSignalsInSec::_1;
}


int Tables::ENUMtoNumSignalsInSec(ENumSignalsInSec::E enumSignalsInSec)
{
    static const int fps[] = {25, 10, 5, 2, 1};
    if(enumSignalsInSec <= ENumSignalsInSec::_1)
    {
        return fps[enumSignalsInSec];
    }
    LOG_ERROR("мЕОПЮБХКЭМШИ ОЮПЮЛЕРП %d", static_cast<int>(enumSignalsInSec));
    return 1;
}


const char* Tables::GetWarning(Warning::E warning)
{
    static pchar warnings[Warning::Count][2] =
    {
        {"опедек йюмюк 1 - бнкэр/дек",      "LIMIT CHANNEL 1 - VOLTS/DIV"},
        {"опедек йюмюк 2 - бнкэр/дек",      "LIMIT CHANNEL 2 - VOLTS/DIV"},
        {"опедек пюгбепрйю - бпелъ/дек",    "LIMIT SWEEP - TIME/DIV"},
        {"бйкчвем охйнбши дерейрнп",        "SET_ENABLED PEAK. DET."},
        {"опедек йюмюк 1 - \x0d",           "LIMIT CHANNEL 1 - \x0d"},
        {"опедек йюмюк 2 - \x0d",           "LIMIT CHANNEL 2 - \x0d"},
        {"опедек пюгбепрйю - спнбемэ",      "LIMIT SWEEP - LEVEL"},
        {"опедек пюгбепрйю - \x97",         "LIMIT SWEEP - \x97"},
        {"охй. дер. ме пюанрюер мю пюгбепрйюу лемее 0.5ЛЙЯ/ДЕК", "PEAK. DET. NOT WORK ON SWEETS LESS THAN 0.5us/div"},
        {"яюлнохяеж ме лнфер пюанрюрэ мю ашярпшу пюгб╗прйюу", "SELF-RECORDER DOES NOT WORK FAST SCANS"},
        {"тюик янупюмем",                   "FILE IS SAVED"},
        {"яхцмюк янупюмем",                 "SIGNAL IS SAVED"},
        {"яхцмкю сдюкем",                   "SIGNAL IS DELETED"},
        {"лемч нркюдйю бйкчвемн",           "MENU DEBUG IS SET_ENABLED"},
        {"онднфдхре меяйнкэйн яейсмд, хдер оепелеыемхе дюммшу", "WAIT ChA FEW SECONDS, GOES ChA MOVEMENT DATA"},
        {"бпелъ ме сярюмнбкемн. лнфере сярюмнбхрэ ецн яеивюя", "TIME IS NOT SET. YOU CAN INSTALL IT NOW"},
        {"яхцмюк ме мюидем",                "SIGNAL IS NOT FOUND"},
        {"мю пюгбепрйюу ледкеммее 10ЛЯ/ДЕК фекюрекэмн сярюмюб-", "AT SCANNER SLOWLY 10ms/div DESIRABLY SET \"SCAN - \x7b\" IN"},
        {"кхбюрэ \"пюгбепрйю - \x7b\" б онкнфемхе \"кЕБН\" дкъ сяйнпе-", "SWITCH \"Left\" FOR TO ACCELERATE THE OUTPUT SIGNAL"},
        {"мхъ бшбндю яхцмюкю",              ""},
        {"нвемэ люкн хглепемхи",            "VERY SMALL MEASURES"},
        {"дкъ бярсокемхъ хглемемхи б яхкс", "FOR THE INTRODUCTION OF CHANGES"},
        {"бшйкчвхре опханп",                "IN FORCE SWITCH OFF THE DEVICE"},
        {"нрйкчвхре бшвхякемхе аот",        "DISCONNECT CALCULATION OF FFT"},
        {"нрйкчвхре люрелюрхвеяйсч тсмйжхч", "DISCONNECT MATHEMATICAL FUNCTION"},
        {"опньхбйю янупюмемю",              "FIRMWARE SAVED"},
        {"оЮЛЪРЭ OTP ОНКМНЯРЭЧ ГЮОНКЕМЮ",   "OPT memory fully completed"}
    };
    return warnings[warning][LANG];
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
    return (chan == ChA) ? 1 : 2;
}


void Tables::DrawStr(int index, int x, int y)
{
    pchar str = symbolsAlphaBet[index];
    if (index == INDEX_SYMBOL)
    {
        Region(Font::GetLengthText(str), 9).Fill(x - 1, y, Color::FLASH_10);
    }
    Text(symbolsAlphaBet[index]).Draw(x, y, index == INDEX_SYMBOL ? Color::FLASH_01 : Color::FILL);
}
