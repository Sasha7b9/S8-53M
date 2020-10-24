#pragma once


struct DataSmallButton
{
    pFuncVV           funcOnPress;    // ��� ������� ��������� ��� ��������� ������� ������. ���������� true, ���� ���� ��
    pFuncVII          funcOnDraw;     // ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    const arrayHints *hintUGO;
};

// ��������� ������ ��� ��������������� ������ ����.
class SmallButton : public Item
{
public:

    static const int WIDTH = 19;
    static const int LEFT = 299;

    SmallButton(const DataItem *const data) : Item(data) {};

    const DataSmallButton *OwnData() const {
        return static_cast<const DataSmallButton *>(data->ad);
    }

    virtual void Draw(int x, int y, bool opened = false);

    virtual void ShortPress();

    void DrawHints(int x, int y, int width);
};