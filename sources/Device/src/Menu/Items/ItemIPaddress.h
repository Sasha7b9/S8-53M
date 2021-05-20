// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DataIPaddress
{
    uint8 *ip0;
    uint8 *ip1;
    uint8 *ip2;
    uint8 *ip3;
    pFuncVB funcOfChanged;
    uint16 *port;
};

class  IPaddress : public Item
{
public:

    static const int OPENED_WIDTH = 34;      // ”величение ширины открытого ip-адреса в случае наличи€ порта

    IPaddress(const DataItem *const data) : Item(data) {};

    const DataIPaddress *OwnData() const {
        return (const DataIPaddress *)(data->ad);
    }

    virtual void Draw(int x, int y, bool opened = false) const;

    // ѕри открытом элементе переставл€ет курсор на следующую позицию
    void NextPosition() const;

    // »змен€ет значение в текущей позиции при открытом элементе
    virtual void OnRegSet(int delta) const;

    void DrawLowPart(int x, int y) const;

    void DrawValue(int x, int y) const;

    void DrawClosed(int x, int y) const;

    void DrawOpened(int x, int y) const;

    // ¬озвращает номер текущего байта (4 - номер порта) и номер текущей позиции в байте.
    static void GetNumPosIPvalue(int *numIP, int *selPos);

    virtual void ShortPress();
};
