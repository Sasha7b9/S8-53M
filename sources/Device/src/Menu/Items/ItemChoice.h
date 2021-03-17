#pragma once


struct DataChoice
{
    pchar    *names;             // �������� ������ �� ������� � ���������� ������.
    int8     *cell;              // ����� ������, � ������� �������� ������� �������� ������.
    pFuncVB	  funcOnChanged;     // ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII  funcForDraw;       // ������� ���������� ����� ��������� ��������. 
};


class Choice : public Item
{
public:
    Choice(const DataItem *const data) : Item(data) {};

    const DataChoice *OwnData() const {
        return static_cast<const DataChoice *>(data->ad);
    }

    pchar NameSubItem(int i) const;
    // ���������� ��� �������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������.
    pchar NameCurrentSubItem() const;
    // ���������� ��� ���������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������.
    pchar NameNextSubItem() const;

    pchar NamePrevSubItem() const;
    // ���������� ���������� ��������� ������ � �������� �� ������ choice.
    int NumSubItems() const;

    void StartChange(int delta);
    // ������������ ��������� ���� ��������.
    float Step() const;
    // �������� �������� choice � ����������� �� �������� � ����� delta.
    void ChangeValue(int delta);

    void FuncOnChanged(bool active) const;

    void FuncForDraw(int x, int y) const;

    virtual void Draw(int x, int y, bool opened = false) const;
    // ��������� ��������� �������
    virtual void ShortPress();

private:
    void DrawClosed(int x, int y) const;

    void DrawOpened(int x, int y) const;
};
