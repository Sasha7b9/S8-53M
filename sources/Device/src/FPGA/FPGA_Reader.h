// 2021/03/17 8:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Mutex_.h"
#include "FPGA/FPGA_Types.h"


struct DataReading;


struct ReaderFPGA
{
    static void ReadData();

    static void ReadPoint();

    static int addition_shift;                  // Дополнительное смещение. Нужно для правильной расстановки точек
                                                // в режиме рандомизатора

    static Mutex mutex_read;                    // Мьютекс на чтение данных

private:

    // Инвертирует данные.
    static void InverseDataIsNecessary(DataReading &data, const Channel &ch);

    static Int CalculateShift();

    // Возвращает адрес, с которого нужно читать первую точку
    static uint16 CalculateAddressRead();

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
            static bool IndexFirstPoint(int *first_out, int *skipped_out);

            // Считать и выбрость num_words данных по адресу address
            static void UtilizeFirstBytes(const uint16 * const address, int num_words);
        };
    };
};
