// 2021/03/18 16:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Buffer_.h"


struct DataStorage
{
    DataStorage();

    const DataSettings &Settings() const;

    // Хотя и возвращается указатель на 8-битные значения, следует иметь ввиду, что в случае, если данные расположены
    // во внешнем ОЗУ, читать следует по 16 бит, т.к. доступ по нечётным адресам к ОЗУ запрещён
    uint8 *Data(const Channel &ch) const;

    // Возвращает true, если в настройках включён хотя бы один канал
    bool HasData() const;

    uint Size() const;

private:

    // В буфере хранятся последовательно - настройки, данные первого канала, данные второго канала
    Buffer buffer;

    uint8 *Begin() const;
};


struct RecordStorage
{
friend class Storage;

    void Fill(const DataStorage &data);
    int Size() const;
    uint8 *Address() const;
private:
    RecordStorage *prev;        // Адрес предыдущей (более старой) записи
    RecordStorage *next;        // Адрес следующей (более новой) записи
    uint16 *dataStorage;        // 
};


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

private:

    // Создаёт запись во внешнем ОЗУ для сохраенения data
    static RecordStorage *Create(const DataStorage &data);

    static RecordStorage *addressFirstRecord;  // Здесь хранится адрес первой записи. Зная его, можно рассчитать все
                                        // остальные адреса
};
