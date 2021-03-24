#pragma once
#include "common/Utils/Containers/String_.h"


struct Float;


namespace GF
{
    String FloatFract2String(float value, bool always_sign = false);

    // alwaysSign - если установлен в true, перед числом всегда будет знак, если установлен в false, только если число
    //              отрицательное
    // numDigits - число разрядов мантиссы
    String Float2String(const Float &value, bool always_sign, int num_digits);

    // Преобразует value в текстовую строку. При этом выводятся все 8 разрядов.
    String Bin2String(uint8 value);

    String Bin2String16(uint16 value);

    String Hex8toString(uint8 value);

    String Int2String(int value, bool always_sign = false, int num_min_fields = 1);

    bool String2Int(char *str, int *value);

    String Voltage2String(const Float &voltage, bool always_sign);

    String Time2String(const Float &time, bool always_sign);

    String Freq2String(const Float &freq, bool always_sign = false);

    String Phase2String(float phase, bool);

    // Преобразует децибелы в текстовую строку.
    String Float2Db(float value, int num_digits);

    // Прибавить к значению по адресу val delta. Если результат не вписывается в диапазон [min; max], ограничить
    void AddLimitationFloat(float *val, float delta, float min, float max);

    void IntToStrCat(char *_buffer, int _value);

    String LogArrayUint8(uint8 *array, uint size);
}
