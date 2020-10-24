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

    static const int OPENED_WIDTH = 34;      // ���������� ������ ��������� ip-������ � ������ ������� �����

    IPaddress(const DataItem *const data) : Item(data) {};

    const DataIPaddress *OwnData() const {
        return static_cast<const DataIPaddress *>(data->ad);
    }

    virtual void Draw(int x, int y, bool opened = false);

    // ��� �������� �������� ������������ ������ �� ��������� �������
    void NextPosition() const;

    // �������� �������� � ������� ������� ��� �������� ��������
    void ChangeValue(int delta);

    void DrawLowPart(int x, int y) const;

    void DrawValue(int x, int y);

    void DrawClosed(int x, int y) const;

    void DrawOpened(int x, int y);

    // ���������� ����� �������� ����� (4 - ����� �����) � ����� ������� ������� � �����.
    static void GetNumPosIPvalue(int *numIP, int *selPos);

    virtual void ShortPress();
};