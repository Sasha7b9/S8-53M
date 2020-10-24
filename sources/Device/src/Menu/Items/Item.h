#pragma once


struct DataItem
{
    TypeItem::E type;
    const Page *keeper;
    pFuncBV     funcOfActive;
    const char **titleHint;
    const void *ad;
};


class Item
{
public:

    static const int TITLE_WIDTH = 79;
    static const int TITLE_HEIGHT = 34;
    static const int HEIGHT = 33;
    static const int WIDTH = TITLE_WIDTH + 1;
    static const int HEIGHT_VALUE = 13;
    static const int OPENED_HEIGHT = 14;
    static const int OPENED_WIDTH = TITLE_WIDTH;
    static const int OPENED_HEIGHT_TITLE = 19;
    static const int WIDTH_VALUE = WIDTH - 4;


    const DataItem *data;

    Item(const DataItem *const data = nullptr);
    // ���������� true, ���� ������, ��������������� ������� �������� ����, ��������� � ������� ���������.
    bool IsPressed() const;
    // ���������� true, ���� ������� ���� item �������� �������, �.�. ����� ���� �����.
    bool IsActive() const;

    TypeItem::E Type() const;
    // ���������� ����� ��������, �������� ����������� ������� �� ������ item.
    Page *Keeper() const;
    // �������/������� ������� ���� �� ������ item.
    void Open(bool open) const;
    // �������/��������� ������� ����� ��������.
    void SetCurrent(bool active) const;
    // ���������� true, ���� ������� ���� �� ������ item ������.
    bool IsOpened() const;

    virtual void ShortPress() {};
    // ��������� �������� ������� �� ������� ���� item.
    virtual void LongPress();

    virtual void Draw(int, int, bool = false) {};

    int OpenedPosY() const;
    // ��������� ��� ����������� �������� Governor, GovernorColor ��� Choice �� ������ item � ����������� �� ����� delta
    void Change(int delta);

    bool ChangeOpened(int delta);
    // ���������� ������ � �������� ��������� �������� Choice ��� NamePage::E �� ������ item.
    int HeightOpened() const;
    // ���������� �������� �������� �� ������ item, ��� ��� �������� �� ������� �������.
    const char *Title() const;

    bool IsPage() const {
        return data->type == TypeItem::Page;
    }
    bool IsChoice() const {
        return data->type == TypeItem::Choice;
    }
    bool IsChoiceReg() const {
        return data->type == TypeItem::ChoiceReg;
    }
    bool IsGovernor() const {
        return data->type == TypeItem::Governor;
    }
    bool IsIP() const {
        return data->type == TypeItem::IP;
    }
    bool IsTime() const {
        return data->type == TypeItem::Time;
    }

    const TimeItem *ReinterpretToTime() const {
        return (TimeItem *)this;
    }
    const Governor *ReinterpretToGovernor() const {
        return (Governor *)this;
    }
    const Choice *ReinterpretToChoice() const {
        return (Choice *)this;
    }
    const Page *ReinterpretToPage() const {
        return (Page *)this;
    }

    static DataItem emptyData;

    static Item empty;

    void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color::E normal, Color::E bright, Color::E dark, bool isPressed) const;

    char GetSymbolForGovernor() const;                        // ���������� ����������� ����������, ��������������� ��� �������� ���������.

protected:
    static int8 gCurDigit;
};
