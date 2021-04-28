// 2021/03/01 10:48:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once



namespace Math
{
    uint ToUINT(const void *pointer);

    uint8 GetMaxFromArray(puchar data, uint first_point, uint last_point);

    uint8 GetMinFromArray(puchar data, uint first_point, uint last_point);

    // Возвращает координату x пересечения линии, проходящей через (x0, y0), (x1, y1), с горизонтальной линией,
    // проходящей через точку с ординатой yHorLine.
    float GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int y_hor_line);

    // Сравнивает два числа. Возвращает true, если числа отличаются друг от друга не более, чем на epsilon_part.
    // При этом для расчёта epsilon_part используется большее в смысле модуля число.
    bool FloatsIsEquals(float val1, float val2, float epsilon_part);

    float RandFloat(float min, float max);

    int Sign(int vlaue);

    // Вычисляет 10**pow.
    int Pow10(int pow);

    // Вычисляет число разрядов в целом типа int.
    int NumDigitsInNumber(int value);

    // Возвращает модуль value.
    int FabsInt(int value);

    uint8 CalculateFiltr(puchar data, int x, int num_points, int num_smoothing);

    void CalculateFiltrArray(puchar data_in, uint8* data_out, int num_points, int num_smoothing);

    template<class T> void Limitation(T *value, T min, T max);
    template<class T> T    Limitation(T value, T min, T max);
    template<class T> void LimitAbove(T *value, T max);
    template<class T> void LimitBelow(T *value, T min);
    template<class T> T    MinFrom2(T val1, T val2);
    template<class T> T    Max(T value1, T value2, T value3);
    template<class T> void Sort(T *value1, T *value2);
    template<class T> bool InRange(T value, T min, T max);

    // Обменивает местами содержимое памяти по адресам value0 и value1
    template<class T> void Swap(T *value0, T *value1);

    // Увеличивает значение по адресу val на 1. Затем, если результат превышает max, приравинвает его к min
    template<class T> T CircleIncrease(T *val, T min, T max);

    // Умеьшает значение по адресу val на 1. Затем, если результат меньше min, приравнивает его max
    template<class T> T CircleDecrease(T *val, T min, T max);

    // Увелечивает значение по адресу val на delta. Затем, если результат больше max, приравнивает его min
    template<class T> T CircleAdd(T *val, T delta, T min, T max);

    // Уменьшает значение по адресу val на delta. Затем, елси результат меньше min, приравнивает его masx
    template<class T> T CircleSub(T *val, T delta, T min, T max);

    // Прибавить к значению по адресу val delta. Если результат не вписывается в диапазон [min; max], ограничить
    template<class T> void AddLimitation(T *val, T delta, T min, T max);

    extern const float Pi;
}
