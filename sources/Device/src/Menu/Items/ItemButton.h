#pragma once


struct DataButton
{
    pFuncVV         funcOnPress;    // �������, ������� ���������� ��� ������� �� ������.
};


// ��������� ������.
class Button : public Item
{
public:
    Button(const DataItem *const data) : Item(data) {};
    const DataButton *OwnData() const {
        return static_cast<const DataButton *>(data->ad);
    }
    virtual void Draw(int x, int y, bool opened = false) const;
    // ��������� ��������� �������
    virtual void ShortPress();

    virtual void LongPress();
};
