#pragma once


struct DataGovernor
{
    int16 *cell;
    int         minValue;               // ���������� ��������, ������� ����� ��������� ���������.
    int16       maxValue;               // ������������ ��������.
    pFuncVV     funcOfChanged;          // �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
    pFuncVV     funcBeforeDraw;         // �������, ������� ���������� ����� ����������
};


// ��������� ���������.
class Governor : public Item
{
public:

    Governor(const DataItem *const data) : Item(data) {};
    const DataGovernor *OwnData() const {
        return static_cast<const DataGovernor *>(data->ad); //-V2571
    }
    // ��������� ��������� ������� �� ������� Governor � ������� governor.
    virtual void ShortPress();
    void StartChange(int detla);        // ��������� �������� �������� ���������� ��� ���������� �������� ���� ���� Governor (� ����������� �� ����� delta)
    float Step() const;                 // ������������ ��������� ���� ��������.
    void  ChangeValue(int delta);       // �������� �������� � ������� ������� ��� ��������� ��������
    int16 NextValue() const;            // ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 PrevValue() const;            // ���������� ��������� ������� ��������, ������� ����� ������� governor.

    virtual void Draw(int x, int y, bool opened = false) const;
    void Draw(int x, int y);

    static Item *address;

private:
    void DrawOpened(int x, int y) const;
    void DrawClosed(int x, int y) const;
    void DrawValue(int x, int y) const;

    // ���������� ����� ��������� � ���� ��� ����� �������� governor. ������� �� ������������� ��������, ������� ����� ��������� governor.
    int  NumDigits() const;
    // ��� �������� �������� ������������ ������ �� ��������� �������
    void NextPosition();
    void DrawLowPart(int x, int y) const;

    static bool inMoveIncrease;
    static bool inMoveDecrease;
    static uint timeStartMS;
};
