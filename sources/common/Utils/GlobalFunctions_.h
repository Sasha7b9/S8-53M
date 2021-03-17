#pragma once
#include "common/Utils/String_.h"


namespace GF
{
    String FloatFract2String(float value, bool always_sign = false);

    // alwaysSign - если установлен в true, перед числом всегда будет знак, если установлен в false, только если число
    //              отрицательное
    // numDigits - число разрядов мантиссы
    String Float2String(float value, bool always_sign, int num_digits);

    // Преобразует value в текстовую строку. При этом выводятся все 8 разрядов.
    String Bin2String(uint8 value);

    String Bin2String16(uint16 value);

    String Hex8toString(uint8 value);

    String Int2String(int value, bool always_sign = false, int num_min_fields = 1);

    bool String2Int(char *str, int *value);

    String Voltage2String(float voltage, bool always_sign);

    char *Time2String(float time, bool always_sign, char buffer[20]);

    char *Freq2String(float freq, bool, char buffer_out[20]);

    char *Phase2String(float phase, bool, char bufferOut[20]);

    // Преобразует децибелы в текстовую строку.
    char *Float2Db(float value, int num_digits, char buffer_out[20]);

    // Возвращает true, если value входит в диапазон [min; max]
    bool IntInRange(int value, int min, int max);

    // Возвращает максимальное значение из трёх
    float MaxFloat(float val1, float val2, float val3);

    // Увеличивает значение по адресу val на 1. Затем, если результат превышает max, приравинвает его к min
    template<class T>
    T CircleIncrease(T *val, T min, T max);

    // Умеьшает значение по адресу val на 1. Затем, если результат меньше min, приравнивает его max
    template<class T>
    T CircleDecrease(T *val, T min, T max);

    // Увелечивает значение по адресу val на delta. Затем, если результат больше max, приравнивает его min
    float CircleAddFloat(float *val, float delta, float min, float max);

    // Уменьшает значение по адресу val на delta. Затем, елси результат меньше min, приравнивает его masx
    float CircleSubFloat(float *val, float delta, float min, float max);

    // Прибавить к значению по адресу val delta. Если результат не вписывается в диапазон [min; max], ограничить
    void AddLimitationFloat(float *val, float delta, float min, float max);

    // Обменивает местами содержимое памяти по адресам value0 и value1
    void SwapInt(int *value0, int *value1);

    // Размещает переменные value0 и value1 в порядке возрастания
    void SortInt(int *value0, int *value1);

    void IntToStrCat(char *_buffer, int _value);

    String LogArrayUint8(uint8 *array, int size);
}
