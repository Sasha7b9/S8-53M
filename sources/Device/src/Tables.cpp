// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Tables.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Settings/Settings.h"


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
