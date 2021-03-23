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
public:

    static const int Y = Grid::TOP + 1;

    inline static int X() {
        return Grid::Right() - Item::TITLE_WIDTH - 1;
    };

    Page(const DataItem *const data, int8 *subPage, int8 *actItem, bool *opened) :
        Item(data), currentSubPage(subPage), posActItem(actItem), actItemIsOpened(opened) {};

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

    void ChangeSubPage(int delta) const;

    virtual void ShortPress();

    void DrawOpened(int yTop);

    const Item *RetLastOpened() const;
    // Вызывает функцию короткого нажатия кнопки над итемом numItem страницы page;
    void ShortPressOnItem(int numItem) const;

    void OpenAndSetItCurrent() const;

    SmallButton *SmallButonFrom(int numButton) const;
    // Возвращает номер текущей подстраницы страницы namePage.
    int GetCurrentSubPage() const;
    // Устанавливает номер текущей подстраницы в странице namePage.
    void SetCurrentSubPage(int posSubPage) const;

    // Возвращает true, если текущий элемент страницы открыт
    bool CurrentItemIsOpened() const;

    // Установить позицию активного пункта на странице namePage.
    void SetPositionActItem(int8 pos);

    // Возвращает позицию активного пункта на странице namePage.
    int8 GetPositionActItem() const;

    void CloseOpenedItem();

    void OpenActItem();

    // Возвращает позицию первого элемента страницы по адресу page на экране. Если текущая подстраница 0, это будет 0, если текущая подстраница 1, это будет 5 и т.д.
    int PosItemOnTop() const;

    bool IsExist() const { return (this != &Page::empty); }

    static Page empty;

private:

    int8 *currentSubPage;                               // Указатель на номер текущей подстраницы
    int8 *posActItem;                                   // Указатель на позицию активного итема
    bool *actItemIsOpened;                              // true, если активный итем раскрыт
    void DrawTitle(int yTop);
    void DrawItems(int yTop) const;
    void DrawPagesUGO(int right, int bottom) const;
};
