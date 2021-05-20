// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DataMACaddress
{
    uint8 *mac0;
    uint8 *mac1;
    uint8 *mac2;
    uint8 *mac3;
    uint8 *mac4;
    uint8 *mac5;
    pFuncVB funcOfChanged;
};


class  MACaddress : public Item
{
public:
    MACaddress(const DataItem *const data) : Item(data) {};

    const DataMACaddress *OwnData() const {
        return (const DataMACaddress *)(data->ad);
    }

    virtual void Draw(int x, int y, bool opened = false) const;

    virtual void OnRegSet(int delta) const;

    void DrawLowPart(int x, int y) const;

    void DrawValue(int x, int y) const;

    void DrawOpened(int x, int y) const;

    void DrawClosed(int x, int y) const;

    virtual void ShortPress();
};
