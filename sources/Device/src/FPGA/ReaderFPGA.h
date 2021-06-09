// 2021/03/17 8:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Mutex_.h"
#include "FPGA/FPGA_Types.h"


struct DataReading;


struct ReaderFPGA
{
    static void ReadData();

    static void ReadPoint();

    // Возвращает адрес, с которого нужно читать первую точку
    static uint16 CalculateAddressRead();

    static Mutex mutex_read;                    // Мьютекс на чтение данных

    struct ADC
    {
        // Читает две точки (из двух АЦП) с учётом калибровок
        static uint16 ReadPoints();

        // Читать массив точек от first до last
        static void ReadPoints(const Channel &ch, void *first, const void *last);

    private:

        static const uint16 *address;       // С этого адреса производится чтение
        static int16  balance;              // Баланс второго АЦП
        static float  stretch;              // Растяжка читаемых данных

        static uint8 StretchOut(uint8 value);   // Растянуть считанное из АЦП значение
    };

private:

    // Инвертирует данные.
    static void InverseDataIsNecessary(DataReading &data, const Channel &ch);

    static Int CalculateShift();

    struct Read
    {
        struct Real
        {
            static void Channel(DataReading &data, const Channel &ch, uint16 addr_read);
        };

        struct Randomizer
        {
            static void Channel(DataReading &data, const Channel &ch, uint16 addr_read);

            // Рассчитывает индекс первой считываемой точки в массиве и также количество точек, которые нужно отбросить
            // при чтении
            // firts_out - в этом место нужно поставить первую считанную точку
            // skipped_out - перед чтением точек нужно отбросить столько точек
            static bool IndexFirstPoint(int *first_out, int *skipped_out);

            // Считать и выбрость num_words данных по адресу address
            static void UtilizeFirstBytes(const uint16 * const address, int num_words);

            // Рассчитывает первый адрес, в который нужно записывать считанные данные
            static uint8 *CalculateFirstAddressWrite(DataReading &dr, const ::Channel &ch);
        };
    };
};
