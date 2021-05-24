// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DataPage
{
    NamePage::E  name;
    const Item **items;
    pFuncVV      funcOnPress;
    pFuncVV      funcOnDraw;
    pFuncVI      funcRegSetSB;
};


// ��������� �������� ����.
class Page : public Item
{
public:

    static const int Y = Grid::TOP + 1;

    inline static int X() {
        return Grid::Right() - Item::TITLE_WIDTH - 1;
    };

    Page(const DataItem *const data, int8 *subPage, int8 *actItem, bool *opened) :
        Item(data), currentSubPage(subPage), posCurrentItem(actItem), currentItemIsOpened(opened) {};

    const DataPage *OwnData() const {
        return (const DataPage *)(data->ad);
    }

    virtual void Draw(int x, int y, bool opened = false) const;
    NamePage::E GetName() const;

    // ���������� ���������� ��������� � �������� �� ������ page.
    int NumItems() const;

    // ���������� ����� �������� ���� �������� ��������.
    const Item *GetItem(int numElement) const;

    // D��������� ����� ���������� � �������� �� ������ page.
    int NumSubPages() const;

    bool IsPageSB() const;

    virtual void OnRegSet(int delta) const;

    virtual void ShortPress();

    void DrawOpened(int yTop);

    const Item *RetLastOpened() const;

    // �������� ������� ��������� ������� ������ ��� ������ numItem �������� page;
    void ShortPressOnItem(int numItem) const;

    void OpenAndSetItCurrent() const;

    SmallButton *SmallButonFrom(int numButton) const;

    // ���������� ����� ������� ����������� �������� namePage.
    int GetCurrentSubPage() const;

    // ������������� ����� ������� ����������� � �������� namePage.
    void SetCurrentSubPage(int posSubPage) const;

    // ���������� ������� ������� �������� �������� �� ������ page �� ������. ���� ������� ����������� 0, ��� ����� 0,
    // ���� ������� ����������� 1, ��� ����� 5 � �.�.
    int PosItemOnTop() const;

    bool IsExist() const { return (this != &Page::empty); }

    void CloseOpenedItem();

    static Page empty;

    // ���������� true, ���� ������� ������� �������� ������
    bool CurrentItemIsOpened() const;

    // ���������� ������� ��������� ������ �� �������� namePage.
    void SetPositionActItem(int8 pos);

    // ���������� ������� ��������� ������ �� �������� namePage.
    int8 GetPositionCurrentItem() const;

    void OpenCurrentItem();

private:

    int8 *currentSubPage;                               // ��������� �� ����� ������� �����������
    int8 *posCurrentItem;                               // ��������� �� ������� �������� �����
    bool *currentItemIsOpened;                          // true, ���� ������� ���� �������
    void DrawTitle(int yTop);
    void DrawItems(int yTop) const;
    void DrawPagesUGO(int right, int bottom) const;
};
