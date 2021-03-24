// 2021/03/18 16:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Buffer_.h"


struct Channel;
struct DataDrawing;
struct DataSettings;
struct RecordStorage;


struct DataDrawingKeeper
{
    DataDrawingKeeper() : data(nullptr) {}
    ~DataDrawingKeeper();
    DataDrawing *data;
};


//----------------------------------------------------------------------------------------------------------------------
struct DataStorage
{
friend struct RecordStorage;

    DataStorage();

    void CreateFromCurrentSettings();

    void CreateFromRecord(RecordStorage *record);

    void CreateNull();

    DataSettings &Settings() const;

    // Хотя и возвращается указатель на 8-битные значения, следует иметь ввиду, что в случае, если данные расположены
    // во внешнем ОЗУ, читать следует по 16 бит, т.к. доступ по нечётным адресам к ОЗУ запрещён
    uint8 *Data(const Channel &ch);

private:

    // В буфере хранятся последовательно - настройки, данные первого канала, данные второго канала
    Buffer buffer;

    uint Size();
};


//----------------------------------------------------------------------------------------------------------------------
struct RecordStorage
{
    /*
    *  Запись записывается так. Сначала идёт собственно RecordStorage, затем DataSettings, а затем данные первого и вто-
    *  рого каналов
    */

friend class Storage;
friend struct DataStorage;

private:

    void Fill(const DataStorage &data);

    uint Size() const;

    // Возвращает размер записи, когда в неё будут записаны данные data
    uint Size(const DataSettings &data) const;

    uint8 *Address() const;

    // Возвращает адрес первого байта, следующего за записью
    uint8 *End() const;

    DataSettings &Data() const;

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
    static bool ExtractLast(DataStorage &data);

    // Извлечь данные, находящиеся на "расстоянии" from_end от последней записи (при from_end возвращает последнюю
    // сохранённую запись)
    static bool Extract(uint from_end, DataStorage &data);

    // Возвращает количество записей в хранилище
    static uint NumRecords();

    static void PrepareDataForDrawing(DataDrawingKeeper &keeper);

private:

    // Создаёт запись во внешнем ОЗУ для сохраенения data
    static RecordStorage *Create(const DataSettings &data);

    // Самая старая запись (записана первой)
    static RecordStorage *Oldest();

    // Самая новая запись (записана последней)
    static RecordStorage *Newest();

    // Удаляет самую старую запись
    static void DeleteOldest();

    // Возвращает false и пустое data (в котором оба канала выключены, остальные настройки не определены)
    static bool CreateNull(DataStorage &data);

    // Найти запись для случая, когда новая запись имеет больший адрес, чем старая
    static RecordStorage *FindForNewestMoreOldest(uint need_memory);

    static RecordStorage *addressOldestRecord;   // Здесь хранится адрес первой записи. Зная его, можно рассчитать все
                                                // остальные адреса
};
