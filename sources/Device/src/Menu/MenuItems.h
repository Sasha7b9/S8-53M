#pragma once
#include "Display/Display.h"
#include "common/Display/Colors_c.h"
#include "Menu/MenuItemsDefs.h"

                                            
#define MAX_NUM_SUBITEMS_IN_CHOICE  12  // ������������ ���������� ��������� ������ � �������� Choice.
#define MAX_NUM_ITEMS_IN_PAGE       15  // ������������ ���������� ������� �� ��������.
#define MENU_ITEMS_ON_DISPLAY       5   // ������� ������� ���� ���������� �� ������ �� ���������.
#define MAX_NUM_CHOICE_SMALL_BUTTON 6   // ������������ ���������� ��������� ��������� ������ + 1


extern int8 gCurDigit;


// ������ ���� ������� ����
struct TypeItem { enum E
{
    None,           
    Choice,        // ����� ������ - ��������� ������� ���� �� ���������� �������� ��������.
    Button,        // ������.
    Page,          // ��������.
    Governor,      // ��������� - ��������� ������� ����� ����� �������� �������� �� ������� ��������� ����������.
    Time,          // ��������� ������ �����.
    IP,            // ��������� ������ IP-�����.
    SwitchButton,
    GovernorColor, // ��������� ������� ����.
    Formula,       // ��������� ������� ���� � ������������ ��� �������������� ������� (��������� � ��������)
    MAC,           // MAC-�����
    ChoiceReg,     // ������� ������, � ������� ����� �������������� �� �������, � ������
    SmallButton,   // ������ ��� ������ ����� ������
    Count
};};

// ����� ID ���� �������������� � ���� �������
struct NamePage { enum E
{
    Display,               // ������
    DisplayAccumulation,   // ������� - ����������
    DisplayAverage,        // ������� - ����������
    DisplayGrid,           // ������� - �����
    DisplayRefresh,        // ������� - ����������
    MainPage,              // ������� �������� ����. �� �� ������ ��� ���������
    Debug,                 // �������
    DebugConsole,          // ������� - �������
    DebugADC,              // ������� - ���
    DebugADCbalance,       // ������� - ��� - ������
    DebugADCstretch,       // ������� - ��� - ��������
    DebugADCrShift,        // ������� - ��� - ���. ��������
    DebugADCaltShift,      // ������� - ��� - ���. ����. ���.
    DebugShowRegisters,    // ������� - ��������
    DebugRandomizer,       // ������� - ����-���
    DebugInformation,      // ������� - ����������
    DebugSound,            // ������� - ����
    DebugAltera,           // ������� - �������
    Channel0,              // ����� 1
    Channel1,              // ����� 2
    Cursors,               // �������
    Trig,                  // �����.
    Memory,                // ������
    MemoryExt,             // ������ - �����. ��
    Measures,              // ���������
    MeasuresField,         // ��������� - ����
    Service,               // ������
    ServiceDisplay,        // ������ - �������
    ServiceDisplayColors,  // ������ - ������� - �����
    ServiceCalibrator,     // ������ - ����������
    ServiceEthernet,       // ������ - ETHERNET
    Help,                  // ������
    Time,                  // ���������
    Math,                  // ������ - ����������
    MathFFT,
    TrigAuto,              // ����� - ��� �����
    SB_Curs,               // ������� �������� ������� - ���������
    SB_MeasTuneMeas,       // ������� �������� ��������� - ���������
    SB_MeasTuneField,      // ������� �������� ��������� - ���� - ������
    SB_MemLatest,          // ������� �������� ������ - ���������
    SB_MemInt,             // ������� �������� ������ - ����� ��
    SB_MemExtSetMask,      // ������� �������� ������ - ����� �� - �����
    SB_MemExtSetName,      // ���������� ����� ����������� ����� �� ������ ��� ������� ����� ����� ��� ��������������� ����� ����� �� - ��� �����
    SB_FileManager,        // ������� �������� ������ - ����� �� - �����
    SB_Help,               // ������� �������� HELP
    SB_MathCursorsFFT,     // ������� �������� ������ - ���������� - �������
    SB_MathFunction,       // ������ - ���������� - �������
    SB_Information,        // ������ - ����������
    SB_SerialNumber,       // ������ - �/�
    Count,
    NoPage
};};             // ��� ���������� ������� ����� �������� ������������� SettingsMenu � SSettings.c

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
    const DataItem *data;

    Item(const DataItem * const data = nullptr);
    // ���������� true, ���� ������, ��������������� ������� �������� ����, ��������� � ������� ���������.
    bool IsPressed() const;

    static DataItem emptyData;

    static Item empty;
};


#define TITLE(item) ((item)->data->titleHint[LANG])
#define HINT(item) ((item)->data->titleHint[2 + LANG])

typedef void * pVOID;
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];


struct DataPage
{
    NamePage::E  name;
    Item       **items;
    pFuncVV      funcOnPress;
    pFuncVV      funcOnDraw;
    pFuncVI      funcRegSetSB;
};


// ��������� �������� ����.
class Page : public Item
{
public:
    Page(const DataItem *const data) : Item(data) {};

    void Draw(int x, int y);
    NamePage::E GetName() const;
    const DataPage *OwnData() const { return static_cast<const DataPage *>(data->ad); }

    static Page empty;
};


struct DataButton
{
    pFuncVV         funcOnPress;    // �������, ������� ���������� ��� ������� �� ������.
};


// ��������� ������.
class Button : public Item
{
public:
    Button(const DataItem *const data) : Item(data) {};
    const DataButton *OwnData() const { return static_cast<const DataButton *>(data->ad); }
    void Draw(int x, int y) const;
};


class StructHelpSmallButton
{
public:
    pFuncVII    funcDrawUGO;    // ��������� �� ������� ��������� ����������� �������� ������
    const char *helpUGO[2];     // ������� � ������� �����������.
};

typedef StructHelpSmallButton arrayHints[MAX_NUM_CHOICE_SMALL_BUTTON];


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

    SmallButton(const DataItem *const data) : Item(data) {};

    void Draw(int x, int y) const;

    const DataSmallButton *OwnData() const { return static_cast<const DataSmallButton *>(data->ad); }
};


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
    void Draw(int x, int y, bool opened) const;
    // ��������� ��������� ������� �� ������� Governor � ������� governor.
    void ShortPress();
    void StartChange(int detla);        // ��������� �������� �������� ���������� ��� ���������� �������� ���� ���� Governor (� ����������� �� ����� delta)
    float Step() const;                 // ������������ ��������� ���� ��������.
    void  ChangeValue(int delta);       // �������� �������� � ������� ������� ��� ��������� ��������
    int16 NextValue() const;            // ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 PrevValue() const;            // ���������� ��������� ������� ��������, ������� ����� ������� governor.
    const DataGovernor *OwnData() const { return static_cast<const DataGovernor *>(data->ad); }

private:
    void DrawOpened(int x, int y) const;
    void DrawClosed(int x, int y) const;
    void DrawValue(int x, int y) const;

    // ���������� ����� ��������� � ���� ��� ����� �������� governor. ������� �� ������������� ��������, ������� ����� ��������� governor.
    int  NumDigits() const;
    // ��� �������� �������� ������������ ������ �� ��������� �������
    void NextPosition();
    void DrawLowPart(int x, int y, bool, bool shade) const;

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

    void Draw(int x, int y, bool opened);

    // ��� �������� �������� ������������ ������ �� ��������� �������
    void NextPosition() const;

    // �������� �������� � ������� ������� ��� �������� ��������
    void ChangeValue(int delta);

    void DrawLowPart(int x, int y, bool, bool shade) const;

    void DrawValue(int x, int y);

    void DrawClosed(int x, int y) const;

    void DrawOpened(int x, int y);

    // ���������� ����� �������� ����� (4 - ����� �����) � ����� ������� ������� � �����.
    static void GetNumPosIPvalue(int *numIP, int *selPos);
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

    void Draw(int x, int y, bool opened);

    void ChangeValue(int delta);

    void DrawLowPart(int x, int y, bool, bool shade) const;

    void DrawValue(int x, int y);

    void DrawOpened(int x, int y);

    void DrawClosed(int x, int y) const;
};

// ��������� ������� ���� ��� ��������� ������������� � ������ �������������� �������
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
    int8    *function;      // ����� ������, ��� �������� Function, �� ������� ������ ���� ��������
    int8    *koeff1add;     // ����� ������������ ��� ������ ����� ��� ��������
    int8    *koeff2add;     // ����� ������������ ��� ������ ����� ��� ��������
    int8    *koeff1mul;     // ����� ������������ ��� ������ ����� ��� ���������
    int8    *koeff2mul;     // ����� ������������ ��� ������ ����� ��� ���������
    int8    *curDigit;      // ������� ������ : 0 - ���� ������� �����, 1 - ����������� ������� �����,
                            //  2 - ���� ������� �����, 3 - ����������� ������� �����
    pFuncVV funcOfChanged;  // ��� ������� ���������� ����� ��������� ��������� �������� ����������.
};


class Formula : public Item
{
public:
    Formula(const DataItem *const data) : Item(data) {};
    const DataFormula *OwnData() const { return reinterpret_cast<const DataFormula *>(data->ad); }
    void Draw(int x, int y, bool opened) const;
private:
    void DrawClosed(int x, int y) const;
    void DrawLowPart(int x, int y, bool, bool shade) const;
    void WriteText(int x, int y, bool) const;
};


struct DataGovernorColor
{
    ColorType *colorType;                  // ��������� ��� �������� �����.
};


class GovernorColor : public Item
{
public:
    GovernorColor(const DataItem *const data) : Item(data) {};
    void Draw(int x, int y, bool opened);

    // �������� ������� ����� � governor
    void ChangeValue(int delta);

    const DataGovernorColor *OwnData() const { return static_cast<const DataGovernorColor *>(data->ad); }

private:
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y, int delta);
};


struct DataChoice
{
    const char **names;             // �������� ������ �� ������� � ���������� ������.
    int8        *cell;              // ����� ������, � ������� �������� ������� �������� ������.
    pFuncVB	     funcOnChanged;     // ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII     funcForDraw;       // ������� ���������� ����� ��������� ��������. 
};


class Choice : public Item
{
public:
    Choice(const DataItem *const data) : Item(data) {};

    const DataChoice *OwnData() const { return static_cast<const DataChoice *>(data->ad); }

    const char *NameSubItem(int i) const;
    // ���������� ��� �������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������.
    const char *NameCurrentSubItem() const;
    // ���������� ��� ���������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������.
    const char *NameNextSubItem() const;

    const char *NamePrevSubItem() const;
    // ���������� ���������� ��������� ������ � �������� �� ������ choice.
    int NumSubItems() const;

    void StartChange(int delta);
    // ������������ ��������� ���� ��������.
    float Step() const;
    // �������� �������� choice � ����������� �� �������� � ����� delta.
    void ChangeValue(int delta);

    void FuncOnChanged(bool active) const;

    void FuncForDraw(int x, int y) const;

    void Draw(int x, int y, bool opened) const;
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
    int8 *curField;       // ������� ���� ���������. 0 - �����, 1 - ���, 2 - ���, 3 - ����, 4 - ����, 5 - �����, 6 - ���, 7 - ����������.
    int8 *hours;
    int8 *minutes;
    int8 *seconds;
    int8 *month;
    int8 *day;
    int8 *year;
};


// ������������� � ���������� �����.
struct TimeItem : public Item
{
    TimeItem(const DataItem *const data) : Item(data) {};

    const DataTime *OwnData() const { return static_cast<const DataTime *>(data->ad); }

    void Draw(int x, int y, bool opened) const;

    void SetOpened();

    void SetNewTime() const;

    void SelectNextPosition();

    void IncCurrentPosition() const;

    void DecCurrentPosition() const;

    void DrawClosed(int x, int y) const;

    void DrawOpened(int x, int y) const;
};

