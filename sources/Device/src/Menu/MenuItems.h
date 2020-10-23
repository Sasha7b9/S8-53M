#pragma once
#include "Display/Display.h"
#include "common/Display/Colors_c.h"
#include "Menu/MenuItemsDefs.h"


class SmallButton;


#define MAX_NUM_CHOICE_SMALL_BUTTON 6    // Максимальное количество вариантов маленькой кнопки + 1

                                            
// Разные виды пунктов меню
struct TypeItem { enum E
{
    None,           
    Choice,        // Пункт выбора - позволяет выбрать одно из нескольких заданных значений.
    Button,        // Кнопка.
    Page,          // Страница.
    Governor,      // Регулятор - позволяет выбрать любое целое числовое значение из заранее заданного диапазаона.
    Time,          // Позволяет ввести время.
    IP,            // Позволяет ввести IP-адрес.
    SwitchButton,
    GovernorColor, // Позволяет выбрать цвет.
    Formula,       // Позволяет выбрать знак и коэффициенты для математической формулы (умножение и сложение)
    MAC,           // MAC-адрес
    ChoiceReg,     // Элемент выбора, в котором выбор осуществляется не кнопкой, а ручкой
    SmallButton,   // Кнопка для режима малых кнопок
    Count
};};

// Здесь ID всех представленных в меню страниц
struct NamePage { enum E
{
    Empty,
    Display,               // ДИСПЕЙ
    DisplayAccumulation,   // ДИСПЛЕЙ - НАКОПЛЕНИЕ
    DisplayAverage,        // ДИСПЛЕЙ - УСРЕДНЕНИЕ
    DisplayGrid,           // ДИСПЛЕЙ - СЕТКА
    DisplayRefresh,        // ДИСПЛЕЙ - ОБНОВЛЕНИЕ
    MainPage,              // Главная страница меню. Из неё растут все остальные
    Debug,                 // ОТЛАДКА
    DebugConsole,          // ОТЛАДКА - КОНСОЛЬ
    DebugADC,              // ОТЛАДКА - АЦП
    DebugADCbalance,       // ОТЛАДКА - АЦП - БАЛАНС
    DebugADCstretch,       // ОТЛАДКА - АЦП - РАСТЯЖКА
    DebugADCrShift,        // ОТЛАДКА - АЦП - ДОП. СМЕЩЕНИЕ
    DebugADCaltShift,      // ОТЛАДКА - АЦП - ДОП. СМЕЩ. ПАМ.
    DebugShowRegisters,    // ОТЛАДКА - РЕГИСТРЫ
    DebugRandomizer,       // ОТЛАДКА - РАНД-ТОР
    DebugInformation,      // ОТЛАДКА - ИНФОРМАЦИЯ
    DebugSound,            // ОТЛАДКА - ЗВУК
    DebugAltera,           // ОТЛАДКА - АЛЬТЕРА
    ChannelA,              // КАНАЛ 1
    ChannelB,              // КАНАЛ 2
    Cursors,               // КУРСОРЫ
    Trig,                  // СИНХР.
    Memory,                // ПАМЯТЬ
    MemoryExt,             // ПАМЯТЬ - ВНЕШН. ЗУ
    Measures,              // ИЗМЕРЕНИЯ
    MeasuresField,         // ИЗМЕРЕНИЯ - ЗОНА
    Service,               // СЕРВИС
    ServiceDisplay,        // СЕРВИС - ДИСПЛЕЙ
    ServiceDisplayColors,  // СЕРВИС - ДИСПЛЕЙ - ЦВЕТА
    ServiceCalibrator,     // СЕРВИС - КАЛИБРАТОР
    ServiceEthernet,       // СЕРВИС - ETHERNET
    Help,                  // ПОМОЩЬ
    Time,                  // РАЗВЕРТКА
    Math,                  // СЕРВИС - МАТЕМАТИКА
    MathFFT,
    TrigAuto,              // СИНХР - АВТ ПОИСК
    SB_Curs,               // Открыта страница КУРСОРЫ - Измерения
    SB_MeasTuneMeas,       // Открыта страница ИЗМЕРЕНИЯ - Настроить
    SB_MeasTuneField,      // Открыта страница ИЗМЕРЕНИЯ - ЗОНА - Задать
    SB_MemLatest,          // Открыта страница ПАМЯТЬ - Последние
    SB_MemInt,             // Открыта страница ПАМЯТЬ - Внутр ЗУ
    SB_MemExtSetMask,      // Открыта страница ПАМЯТЬ - ВНЕШН ЗУ - Маска
    SB_MemExtSetName,      // Появляется перед сохранением файла на флешку для задания имени файла при соответствующей опции ВНЕШН ЗУ - Имя файла
    SB_FileManager,        // Открыта страница ПАМЯТЬ - ВНЕШН ЗУ - Обзор
    SB_Help,               // Открыта страница HELP
    SB_MathCursorsFFT,     // Открыта страница СЕРВИС - Математика - Курсоры
    SB_MathFunction,       // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ
    SB_Information,        // СЕРВИС - ИНФОРМАЦИЯ
    SB_SerialNumber,       // ОТАДКА - С/Н
    Count,
    NoPage
};};             // При добавлении страниц нужно изменять инициализацию SettingsMenu в SSettings.c

class Page;


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
    static const int HEIGHT_VALUE = 13;
    static const int OPENED_HEIGHT = 14;
    static const int OPENED_HEIGHT_TITLE = 19;

    const DataItem *data;

    Item(const DataItem * const data = nullptr);
    // Возвращает true, если кнопка, соответствующая данному элементу меню, находится в нажатом положении.
    bool IsPressed() const;
    // Вовзращает true, если элемент меню item является ативным, т.е. может быть нажат.
    bool IsActive() const;

    TypeItem::E Type() const;
    // Возвращает адрес элемента, которому принадлежит элемент по адресу item.
    Page *Keeper() const;
    // Открыть/закрыть элемент меню по адрему item.
    void Open(bool open) const;
    // Сделать/разделать текущим пункт страницы.
    void SetCurrent(bool active) const;
    // Возвращает true, если элемент меню по адрему item открыт.
    bool IsOpened() const;

    virtual void ShortPress() {};
    // Обработка длинного нажатия на элемент меню item.
    virtual void LongPress();

    virtual void Draw(int, int, bool = false) {};

    int OpenedPosY() const;
    // Уменьшает или увеличивает значение Governor, GovernorColor или Choice по адресу item в зависимости от знака delta
    void Change(int delta);

    bool ChangeOpened(int delta);
    // Возвращает высоту в пикселях открытого элемента Choice или NamePage::E по адресу item.
    int HeightOpened() const;
    // Возвращает название элемента по адресу item, как оно выглядит на дисплее прибора.
    const char *Title() const;

    bool IsPage() const { return Type() == TypeItem::Page; }

    static DataItem emptyData;

    static Item empty;

protected:
    static int8 gCurDigit;
};


#define TITLE(item) ((item)->data->titleHint[LANG])
#define HINT(item) ((item)->data->titleHint[2 + LANG])

struct DataPage
{
    NamePage::E  name;
    Item       **items;
    pFuncVV      funcOnPress;
    pFuncVV      funcOnDraw;
    pFuncVI      funcRegSetSB;
};


// Описывает страницу меню.
class Page : public Item
{
public:
    Page(const DataItem *const data) : Item(data) {};
    const DataPage *OwnData() const { return static_cast<const DataPage *>(data->ad); }
    virtual void Draw(int x, int y, bool opened = false);
    NamePage::E GetName() const;
    // Возвращает количество элементов в странице по адресу page.
    int NumItems() const;
    // Возвращает адрес элемента меню заданной страницы.
    Item *GetItem(int numElement) const;
    // Dозвращает число подстраниц в странице по адресу page.
    int NumSubPages() const;

    bool IsSB() const;
    // Возвращает позицию текущего элемента странцы page.
    int8 PosCurrentItem() const;

    void ChangeSubPage(int delta) const;

    virtual void ShortPress();

    void DrawOpened(int yTop);

    Item *RetLastOpened(TypeItem::E *_type);
    // Вызывает функцию короткого нажатия кнопки над итемом numItem страницы page;
    void ShortPressOnItem(int numItem) const;

    void OpenAndSetItCurrent() const;
    
    SmallButton *SmallButonFrom(int numButton) const;
    // Возвращает номер текущей подстраницы страницы namePage.
    int GetCurrentSubPage() const;
    // Устанавливает номер текущей подстраницы в странице namePage.
    void SetCurrentSubPage(int posSubPage) const;

    // Возвращает true, если текущий элемент страницы с именем namePage открыт.
    bool CurrentItemIsOpened();

    // Установить позицию активного пункта на странице namePage.
    void SetPosActItem(int8 pos);

    // Возвращает позицию активного пункта на странице namePage.
    int GetPosActItem() const;

    // Возвращает позицию первого элемента страницы по адресу page на экране. Если текущая подстраница 0, это будет 0, если текущая подстраница 1, это будет 5 и т.д.
    int PosItemOnTop() const;

    bool IsExist() const { return (this != &Page::empty); }

    static Page empty;

private:
    void DrawTitle(int yTop);
    void DrawItems(int yTop) const;
    void DrawPagesUGO(int right, int bottom) const;
};


struct DataButton
{
    pFuncVV         funcOnPress;    // Функция, которая вызывается при нажатии на кнопку.
};


// Описывает кнопку.
class Button : public Item
{
public:
    Button(const DataItem *const data) : Item(data) {};
    const DataButton *OwnData() const { return static_cast<const DataButton *>(data->ad); }
    virtual void Draw(int x, int y, bool opened = false);
    // Обработка короткого нажатия
    virtual void ShortPress();

    virtual void LongPress();
};


class StructHelpSmallButton
{
public:
    pFuncVII    funcDrawUGO;    // Указатель на функцию отрисовки изображения варианта кнопки
    const char *helpUGO[2];     // Подпись к данному изображению.
};

typedef StructHelpSmallButton arrayHints[MAX_NUM_CHOICE_SMALL_BUTTON];


struct DataSmallButton
{
    pFuncVV           funcOnPress;    // Эта функция вызвается для обработки нажатия кнопки. Возвращает true, если надо за
    pFuncVII          funcOnDraw;     // Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const arrayHints *hintUGO;
};

// Описывает кнопку для дополнительного режима меню.
class SmallButton : public Item
{
public:

    static const int WIDTH = 19;

    SmallButton(const DataItem *const data) : Item(data) {};

    const DataSmallButton *OwnData() const { return static_cast<const DataSmallButton *>(data->ad); }

    virtual void Draw(int x, int y, bool opened = false);

    virtual void ShortPress();

    void DrawHints(int x, int y, int width);
};


struct DataGovernor
{
    int16 *cell;
    int         minValue;               // Минмальное значение, которое может принимать регулятор.
    int16       maxValue;               // Максимальное значение.
    pFuncVV     funcOfChanged;          // Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV     funcBeforeDraw;         // Функция, которая вызывается перед отрисовкой
};


// Описывает регулятор.
class Governor : public Item
{ 
public:

    Governor(const DataItem *const data) : Item(data) {};
    const DataGovernor *OwnData() const { return static_cast<const DataGovernor *>(data->ad); }
    // Обработка короткого нажатия на элемент Governor с адресом governor.
    virtual void ShortPress();
    void StartChange(int detla);        // Запускает процессс анимации инкремента или декремента элемента меню типа Governor (в зависимости от знака delta)
    float Step() const;                 // Рассчитывате следующий кадр анимации.
    void  ChangeValue(int delta);       // Изменяет значение в текущей позиции при раскрытом элементе
    int16 NextValue() const;            // Возвращает следующее большее значение, которое может принять governor.
    int16 PrevValue() const;            // Возвращает следующее меньшее значение, которое может принять governor.

    virtual void Draw(int x, int y, bool opened = false);
    void Draw(int x, int y);

    static Item *address;

private:
    void DrawOpened(int x, int y) const;
    void DrawClosed(int x, int y) const;
    void DrawValue(int x, int y) const;

    // Возвращает число знакомест в поле для ввода элемента governor. Зависит от максимального значения, которое может принимать governor.
    int  NumDigits() const;
    // При открытом элементе переставляет курсор на следующую позицию
    void NextPosition();
    void DrawLowPart(int x, int y) const;

    static bool inMoveIncrease;
    static bool inMoveDecrease;
    static uint timeStartMS;
};


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

    IPaddress(const DataItem *const data) : Item(data) {};

    const DataIPaddress *OwnData() const { return static_cast<const DataIPaddress *>(data->ad); }

    virtual void Draw(int x, int y, bool opened = false);

    // При открытом элементе переставляет курсор на следующую позицию
    void NextPosition() const;

    // Изменяет значение в текущей позиции при открытом элементе
    void ChangeValue(int delta);

    void DrawLowPart(int x, int y) const;

    void DrawValue(int x, int y);

    void DrawClosed(int x, int y) const;

    void DrawOpened(int x, int y);

    // Возвращает номер текущего байта (4 - номер порта) и номер текущей позиции в байте.
    static void GetNumPosIPvalue(int *numIP, int *selPos);

    virtual void ShortPress();
};


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

    const DataMACaddress *OwnData() const { return static_cast<const DataMACaddress *>(data->ad); }

    virtual void Draw(int x, int y, bool opened = false);

    void ChangeValue(int delta);

    void DrawLowPart(int x, int y) const;

    void DrawValue(int x, int y);

    void DrawOpened(int x, int y);

    void DrawClosed(int x, int y) const;

    virtual void ShortPress();
};

// Описывает элемент меню для установки коэффициентов и знаков математической формулы
#define FIELD_SIGN_MEMBER_1_ADD 0
#define FIELD_SIGN_MEMBER_1_MUL 1
#define FIELD_SIGN_MEMBER_2_ADD 2
#define FIELD_SIGN_MEMBER_2_MUL 3
#define POS_SIGN_MEMBER_1       0
#define POS_KOEFF_MEMBER_1      1
#define POS_SIGN_MEMBER_2       2
#define POS_KOEFF_MEMBER_2      3


struct DataFormula
{
    int8    *function;      // Адрес ячейки, где хранится Function, из которой берётся знак операции
    int8    *koeff1add;     // Адрес коэффициента при первом члене для сложения
    int8    *koeff2add;     // Адрес коэффициента при втором члене для сложения
    int8    *koeff1mul;     // Адрес коэффициента при первом члене для умножения
    int8    *koeff2mul;     // Адрес коэффициента при втором члене для умножения
    int8    *curDigit;      // Текущий разряд : 0 - знак первого члена, 1 - коэффициент первого члена,
                            //  2 - знак второго члена, 3 - коэффициент второго члена
    pFuncVV funcOfChanged;  // Эта функция вызывается после изменения состояния элемента управления.
};


class Formula : public Item
{
public:
    Formula(const DataItem *const data) : Item(data) {};
    const DataFormula *OwnData() const { return reinterpret_cast<const DataFormula *>(data->ad); }
    virtual void Draw(int x, int y, bool opened = false);
private:
    void DrawClosed(int x, int y) const;
    void DrawLowPart(int x, int y) const;
    void WriteText(int x, int y, bool) const;
};


struct DataGovernorColor
{
    ColorType *colorType;                  // Структура для описания цвета.
};


class GovernorColor : public Item
{
public:
    GovernorColor(const DataItem *const data) : Item(data) {};

    const DataGovernorColor *OwnData() const { return static_cast<const DataGovernorColor *>(data->ad); }

    virtual void Draw(int x, int y, bool opened = false);
    // Изменить яркость цвета в governor
    void ChangeValue(int delta);

    virtual void ShortPress();

private:
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y, int delta);
};


struct DataChoice
{
    const char **names;             // Варианты выбора на русском и английском языках.
    int8        *cell;              // Адрес ячейки, в которой хранится позиция текущего выбора.
    pFuncVB	     funcOnChanged;     // Функция должна вызываться после изменения значения элемента.
    pFuncVII     funcForDraw;       // Функция вызывается после отрисовки элемента. 
};


class Choice : public Item
{
public:
    Choice(const DataItem *const data) : Item(data) {};

    const DataChoice *OwnData() const { return static_cast<const DataChoice *>(data->ad); }

    const char *NameSubItem(int i) const;
    // Возвращает имя текущего варианта выбора элемента choice, как оно записано в исходном коде программы.
    const char *NameCurrentSubItem() const;
    // Возвращает имя следующего варианта выбора элемента choice, как оно записано в исходном коде программы.
    const char *NameNextSubItem() const;

    const char *NamePrevSubItem() const;
    // Возвращает количество вариантов выбора в элементе по адресу choice.
    int NumSubItems() const;

    void StartChange(int delta);
    // Рассчитывает следующий кадр анимации.
    float Step() const;
    // Изменяет значение choice в зависимости от величины и знака delta.
    void ChangeValue(int delta);

    void FuncOnChanged(bool active) const;

    void FuncForDraw(int x, int y) const;

    virtual void Draw(int x, int y, bool opened = false);
    // Обработка короткого нажатия
    virtual void ShortPress();

private:
    void DrawClosed(int x, int y) const;

    void DrawOpened(int x, int y) const;
};


#define iEXIT   0
#define iDAY    1
#define iMONTH  2
#define iYEAR   3
#define iHOURS  4
#define iMIN    5
#define iSEC    6
#define iSET    7


struct DataTime
{
    int8 *curField;       // Текущее поле установки. 0 - выход, 1 - сек, 2 - мин, 3 - часы, 4 - день, 5 - месяц, 6 - год, 7 - установить.
    int8 *hours;
    int8 *minutes;
    int8 *seconds;
    int8 *month;
    int8 *day;
    int8 *year;
};


// Устанавливает и показывает время.
struct TimeItem : public Item
{
    TimeItem(const DataItem *const data) : Item(data) {};

    const DataTime *OwnData() const { return static_cast<const DataTime *>(data->ad); }

    virtual void Draw(int x, int y, bool opened = false);

    void SetOpened();

    void SetNewTime() const;

    void SelectNextPosition();

    void IncCurrentPosition() const;

    void DecCurrentPosition() const;

    void DrawClosed(int x, int y) const;

    void DrawOpened(int x, int y) const;

    virtual void ShortPress();

    virtual void LongPress();
};

