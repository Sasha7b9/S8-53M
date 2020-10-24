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
        return static_cast<const DataMACaddress *>(data->ad);
    }

    virtual void Draw(int x, int y, bool opened = false);

    void ChangeValue(int delta);

    void DrawLowPart(int x, int y) const;

    void DrawValue(int x, int y);

    void DrawOpened(int x, int y);

    void DrawClosed(int x, int y) const;

    virtual void ShortPress();
};
