// 2021/03/18 16:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Buffer_.h"


//----------------------------------------------------------------------------------------------------------------------
struct DataStorage
{
friend struct RecordStorage;

    DataStorage();

    const DataSettings &Settings() const;

    // Хотя и возвращается указатель на 8-битные значения, следует иметь ввиду, что в случае, если данные расположены
    // во внешнем ОЗУ, читать следует по 16 бит, т.к. доступ по нечётным адресам к ОЗУ запрещён
    uint8 *Data(const Channel &ch) const;

private:

    // В буфере хранятся последовательно - настройки, данные первого канала, данные второго канала
    Buffer buffer;

    uint8 *Begin() const;

    uint Size() const;
};


//----------------------------------------------------------------------------------------------------------------------
struct RecordStorage
{
friend class Storage;

private:

    void Fill(const DataStorage &data);

    uint Size() const;

    // Возвращает размер записи, когда в неё будут записаны данные data
    uint Size(const DataStorage &data) const;

    uint8 *Address() const;

    // Возвращает адрес первого байта, следующего за записью
    uint8 *End() const;

    DataStorage &Data() const;

    RecordStorage *prev;        // Адрес предыдущей (более старой) записи
    RecordStorage *next;        // Адрес следующей (более новой) записи
};


//----------------------------------------------------------------------------------------------------------------------
class Storage
{
    /*
    *  Класс организует из свободного пространства внешнего ОЗУ хранилище данных, данные в которым хранятся так.
    *  В переменной addressFirstRecord хранится адрес первой записи. Запись представляет собой элемент типа DataStorage.
    *  Начало каждой последующей записи находится за последним байтом предыдущей записи.
    *  При достижении конца ОЗУ стирается первый (самый старый) элемент, и на его место записывается последний (самый
    *  новый. Не забыть переписать указатель на первый э
    */

public:

    static void Append(const DataStorage &data);

    // Извлечь последние положенные данные
    static DataStorage &ExtractLast();

    // Извлечь данные, находящиеся на "расстоянии" from_end от последней записи (при from_end возвращает последнюю
    // сохранённую запись)
    static DataStorage &Extract(uint from_end);

    // Возвращает количество записей в хранилище
    static uint NumRecords();

private:

    // Создаёт запись во внешнем ОЗУ для сохраенения data
    static RecordStorage *Create(const DataStorage &data);

    // Самая старая запись (записана первой)
    static RecordStorage *Oldest();

    // Самая новая запись (записана последней)
    static RecordStorage *Newest();

    // Удаляет самую старую запись
    static void DeleteOldest();

    static RecordStorage *addressOldestRecord;   // Здесь хранится адрес первой записи. Зная его, можно рассчитать все
                                                // остальные адреса

    static DataStorage *nullData;
};
