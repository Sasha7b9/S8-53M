#pragma once


namespace GF
{
    char *FloatFract2String(float value, bool always_sign, char out_buffer[20]);

    // alwaysSign - если установлен в true, перед числом всегда будет знак, если установлен в false, только если число
    //              отрицательное
    // numDigits - число разрядов мантиссы
    char *Float2String(float value, bool always_sign, int num_digits, char out_buffer[20]);

    // Преобразует value в текстовую строку. При этом выводятся все 8 разрядов.
    char *Bin2String(uint8 value, char out_buffer[9]);

    char *Bin2String16                            // Преобразует value в текстовую строку
                                                    // @attention Строка будте храниться до следующего вызова функции. Если рузультат нужен большее количество времени, то его нужно скопировать себе.
                                                    (uint16 value,
                                                        char buffer[19]);
    char *Hex8toString                            // Преобразует value в текстовую строку в шестнадцатиричном виде.
                                                    // @attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопировать себе.
                                                    (uint8 value,
                                                        char buffer[3]);
    char *Int2String                              // Преобразует value в текстовую строку.
                                                    // @attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопировать себе.
                                                    (int value,                 // значение.
                                                        bool alwaysSign,            // если установлен в true, знак будет выводиться всегда.
                                                        int numMinFields,           // минимальное число занимаемых знакомест. Если для вывода числа столько не требуется, лишние заполняются нулями.
                                                        char buffer[20]
                                                    );
    bool    String2Int(char *str, int *value);
    char *Voltage2String                          // Преобразует voltage вольт в текстовую строку.
                                                    // @attention Страка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопитровать себе.
                                                    (float voltage,         // значение напряжения в вольтах.
                                                        bool alwaysSign,        // если установлено в true, знак выводится всегда.
                                                        char buffer[20]
                                                    );
    char *Time2String                             // Преобразует time секунд в текстовую строку.
    (float time,                // время в секундах. Может быть отрицательным.
        bool alwaysSign,            // если установлен в true, знак отображается всегда.
        char buffer[20]
    );
    char *Freq2String(float freq, bool, char bufferOut[20]);      // Преобразует freq герц в текстовую строку.
    char *Phase2String(float phase, bool, char bufferOut[20]);    // Преобразует градусы в строку.
    char *Float2Db(float value, int numDigits, char bufferOut[20]);   // Преобразует децибелы в текстовую строку.
    bool    IntInRange                              // Возвращает true, если value входит в диапазон [min; max].
    (int value,                  // проверяемое значение.
        int min,                     // нижняя граница диапазона.
        int max                      // верхняя граница диапазона.
    );
    float   MaxFloat(float val1, float val2, float val3);                   // Возвращает максимальное значение из трёх.
    int8    CircleIncreaseInt8(int8 *val, int8 min, int8 max);              // Увеличивает значение по адресу val на 1. Затем, если результат превышает max, приравинвает его к min.
    int16   CircleIncreaseInt16(int16 *val, int16 min, int16 max);
    int8    CircleDecreaseInt8(int8 *val, int8 min, int8 max);              // Умеьшает значение по адресу val на 1. Затем, если результат меньше min, приравнивает его max.
    int16   CircleDecreaseInt16(int16 *val, int16 min, int16 max);
    int     CircleIncreaseInt(int *val, int min, int max);
    int     CircleDecreaseInt(int *val, int min, int max);
    float   CircleAddFloat(float *val, float delta, float min, float max);  // Увелечивает значение по адресу val на delta. Затем, если результат больше max, приравнивает его min.
    float   CircleSubFloat(float *val, float delta, float min, float max);  // Уменьшает значение по адресу val на delta. Затем, елси результат меньше min, приравнивает его masx.
    void    AddLimitationFloat                                              // Прибавить к значению по адресу val delta. Если результат не вписывается в диапазон [min; max], ограничить.
    (float *val,                                // по этому адресу хранится значение.
        float delta,                                // а это дельта.
        float min,                                  // нижняя граница диапазона.
        float max                                   // верхняя граница диапазона.
    );
    void    SwapInt(int *value0, int *value1);                      // Обменивает местами содержимое памяти по адресам value0 и value1.
    void    SortInt(int *value0, int *value1);                      // Размещает переменные value0 и value1 в порядке возрастания.
    void    IntToStrCat(char *_buffer, int _value);
    char *PrintArrayUint8(uint8 *array, int size, char *out_buffer);
}
