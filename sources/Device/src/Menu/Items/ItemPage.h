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


// Описывает страницу меню.
class Page : public Item
{
    friend class Item;

public:

    static const int Y = Grid::TOP + 1;

    int X() const;

    Page(const DataItem *const data, int8 *subPage, int8 *actItem, bool *opened) :
        Item(data), currentSubPage(subPage), posCurrentItem(actItem), currentItemIsOpened(opened) {};

    const DataPage *OwnData() const {
        return (const DataPage *)(data->ad);
    }

    virtual void Draw(int x, int y, bool opened = false) const;
    NamePage::E GetName() const;

    // Возвращает количество элементов в странице по адресу page.
    int NumItems() const;

    // Возвращает адрес элемента меню заданной страницы.
    const Item *GetItem(int numElement) const;

    // Dозвращает число подстраниц в странице по адресу page.
    int NumSubPages() const;

    bool IsPageSB() const;

    virtual void OnRegSet(int delta) const;

    virtual void ShortPress();

    void DrawOpened(int yTop);

    // Возвращает адрес последнего раскрытого итема
    const Item *RetLastOpened() const;

    // Вызывает функцию короткого нажатия кнопки над итемом numItem страницы page;
    void ShortPressOnItem(int numItem) const;

    void OpenAndSetItCurrent() const;

    SmallButton *SmallButonFrom(int numButton) const;

    // Возвращает номер текущей подстраницы страницы namePage.
    int GetCurrentSubPage() const;

    // Устанавливает номер текущей подстраницы в странице namePage.
    void SetCurrentSubPage(int posSubPage) const;

    // Возвращает позицию первого элемента страницы по адресу page на экране. Если текущая подстраница 0, это будет 0,
    // если текущая подстраница 1, это будет 5 и т.д.
    int PosItemOnTop() const;

    bool IsExist() const { return (this != &Page::empty); }

    void CloseOpenedItem();

    static Page empty;

    const Item *CurrentItem(bool trace = false) const;

    void SetCurrentItem(const Item *item);


protected:

    int8 *currentSubPage;                           // Указатель на номер текущей подстраницы
    int8 *posCurrentItem;                           // Указатель на позицию текущего итема. Если == -1, нет текущего
    bool *currentItemIsOpened;                      // true, если текущий итем раскрыт
    void DrawTitle(int yTop);
    void DrawItems(int yTop) const;
    void DrawPagesUGO(int right, int bottom) const;
};
