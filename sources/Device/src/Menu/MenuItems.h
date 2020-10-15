#pragma once
#include "Display/Display.h"
#include "common/Display/Colors.h"

                                            
#define MAX_NUM_SUBITEMS_IN_CHOICE  12  // ������������ ���������� ��������� ������ � �������� Choice.
#define MAX_NUM_ITEMS_IN_PAGE       15  // ������������ ���������� ������� �� ��������.
#define MENU_ITEMS_ON_DISPLAY       5   // ������� ������� ���� ���������� �� ������ �� ���������.
#define MAX_NUM_CHOICE_SMALL_BUTTON 6   // ������������ ���������� ��������� ��������� ������ + 1


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

#define COMMON_PART_MENU_ITEM                                                                                     \
    TypeItem::E type;             /* ��� ����� */                                                     \
    const Page* keeper;           /* ����� ��������, ������� �����������. ��� NamePage::MainPage = 0 */    \
    pFuncBV     funcOfActive;     /* ������� �� ������ ������� */                                     \
    const char* titleHint[4];     /* �������� �������� �� ������� � ���������� ������. ����� ��������� ��� ������ ������ */



struct ControlStruct
{
    COMMON_PART_MENU_ITEM
};
class Control
{
public:
    COMMON_PART_MENU_ITEM

public:
    Control(const ControlStruct *str);
    Control(TypeItem::E type_, const Page* keeper_, pFuncBV funcOfActive_, const char *tileRU, const char *titleEN, const char *hintRU, const char *hintEN);
    // ���������� true, ���� ������, ��������������� ������� �������� ����, ��������� � ������� ���������.
    bool IsPressed() const;
};


#define TITLE(item) ((item)->titleHint[LANG])
#define HINT(item) ((item)->titleHint[2 + LANG])

typedef void * pVOID;
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];


struct PageStruct
{
    COMMON_PART_MENU_ITEM
    NamePage::E         name;
    const arrayItems    items;
    pFuncVV             funcOnPress;
    pFuncVV             funcOnDraw;
    pFuncVI             funcRegSetSB;
};

// ��������� �������� ����.
class Page : public     Control
{
public:
    NamePage::E         name;                               // ��� �� ������������ NamePage
    const arrayItems   *items;                              // ����� ��������� �� ������ ���� �������� (� ������� ��������)
                                                            // ��� �������� ����� ������  ����� �������� 6 ���������� �� SmallButton : 0 - Key::Menu, 1...5 - Key::F1...Key::F5
    pFuncVV             funcOnPress;                        // ����� ���������� ��� ������� �� �������� ��������
    pFuncVV             funcOnDraw;                         // ����� ���������� ����� ��������� ������
    pFuncVI             funcRegSetSB;                       // � �������� ����� ������ ���������� ��� �������� ����� ���������

    //Page(PageStruct *pageStruct);
    
    Page(const Page *keeper_, pFuncBV funcOfActive_,
         const char *titleRU, const char *titleEN, const char *hintRU, const char *hintEN, NamePage::E name_,
         const arrayItems *items_, pFuncVV funcOnPress_ = 0, pFuncVV funcOnDraw_ = 0, pFuncVI funcRegSetSB_ = 0);
    void Draw(int x, int y);
    NamePage::E GetName() const;
};


// ��������� ������.
class Button : public Control
{
public:
    pFuncVV         funcOnPress;    // �������, ������� ���������� ��� ������� �� ������.

    Button(const Page *keeper_, pFuncBV funcOfActive_,
            const char *titleRU, const char *titleEN, const char *hintRU, const char *hintEN, pFuncVV funcOnPress_);
    void Draw(int x, int y) const;
};


class StructHelpSmallButton
{
public:
    pFuncVII    funcDrawUGO;    // ��������� �� ������� ��������� ����������� �������� ������
    const char *helpUGO[2];     // ������� � ������� �����������.
};

typedef StructHelpSmallButton arrayHints[MAX_NUM_CHOICE_SMALL_BUTTON];


// ��������� ������ ��� ��������������� ������ ����.
class SmallButton : public Control
{
public:
    pFuncVV             funcOnPress;    // ��� ������� ��������� ��� ��������� ������� ������. ���������� true, ���� ���� ��
    pFuncVII            funcOnDraw;     // ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    const arrayHints   *hintUGO;

    SmallButton(const Page *keeper_, pFuncBV funcOfActive_,
                const char *titleRU, const char *titleEN, const char *hintRU, const char *hintEN,
                pFuncVV funcOnPress_, pFuncVII funcOnDraw_, const arrayHints *hintUGO_ = 0);
    void Draw(int x, int y) const;
};


// ��������� ���������.
class Governor : public Control
{ 
public:
    int16*      cell;
    int         minValue;               // ���������� ��������, ������� ����� ��������� ���������.
    int16       maxValue;               // ������������ ��������.
    pFuncVV     funcOfChanged;          // �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
    pFuncVV     funcBeforeDraw;         // �������, ������� ���������� ����� ����������

    Governor(const Page *keeper_, pFuncBV funcOfActive_,
             const char *titleRU, const char *titleEN, const char *hintRU, const char *hintEN,
             int16 *cell_, int16 minValue_, int16 maxValue_, pFuncVV funcOfChanged_ = 0, pFuncVV funcBeforeDraw_ = 0);
    void Draw(int x, int y, bool opened) const;
    // ��������� ��������� ������� �� ������� Governor � ������� governor.
    void ShortPress();
    void StartChange(int detla);        // ��������� �������� �������� ���������� ��� ���������� �������� ���� ���� Governor (� ����������� �� ����� delta)
    float Step() const;                 // ������������ ��������� ���� ��������.
    void  ChangeValue(int delta);       // �������� �������� � ������� ������� ��� ��������� ��������
    int16 NextValue() const;            // ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 PrevValue() const;            // ���������� ��������� ������� ��������, ������� ����� ������� governor.

private:
    void DrawOpened(int x, int y) const;
    void DrawClosed(int x, int y) const;
    void DrawValue(int x, int y) const;

    // ���������� ����� ��������� � ���� ��� ����� �������� governor. ������� �� ������������� ��������, ������� ����� ��������� governor.
    int  NumDigits() const;
    // ��� �������� �������� ������������ ������ �� ��������� �������
    void NextPosition();

};


struct IPaddressStruct
{
    ControlStruct str;
    uint8* ip0;
    uint8* ip1;
    uint8* ip2;
    uint8* ip3;
    pFuncVB funcOfChanged;
    uint16 *port;
};

class  IPaddress : public Control
{
public:
    uint8*  ip0;
    uint8*  ip1;
    uint8*  ip2;
    uint8*  ip3;
    pFuncVB funcOfChanged;
    uint16* port;

    IPaddress(const IPaddressStruct *str);

    void Draw(int x, int y, bool opened);

    // ��� �������� �������� ������������ ������ �� ��������� �������
    void NextPosition() const;

    // �������� �������� � ������� ������� ��� �������� ��������
    void ChangeValue(int delta);

    // ���������� ����� �������� ����� (4 - ����� �����) � ����� ������� ������� � �����.
    static void GetNumPosIPvalue(int *numIP, int *selPos);
};


class  MACaddress
{
public:
    COMMON_PART_MENU_ITEM
    uint8*  mac0;
    uint8*  mac1;
    uint8*  mac2;
    uint8*  mac3;
    uint8*  mac4;
    uint8*  mac5;
    pFuncVB funcOfChanged;

    void Draw(int x, int y, bool opened);

    void ChangeValue(int delta);
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

class Formula
{
public:
    COMMON_PART_MENU_ITEM
    int8*           function;                   // ����� ������, ��� �������� Function, �� ������� ������ ���� ��������
    int8*           koeff1add;                  // ����� ������������ ��� ������ ����� ��� ��������
    int8*           koeff2add;                  // ����� ������������ ��� ������ ����� ��� ��������
    int8*           koeff1mul;                  // ����� ������������ ��� ������ ����� ��� ���������
    int8*           koeff2mul;                  // ����� ������������ ��� ������ ����� ��� ���������
    int8*           curDigit;                   // ������� ������ : 0 - ���� ������� �����, 1 - ����������� ������� �����,
                                                //  2 - ���� ������� �����, 3 - ����������� ������� �����
    pFuncVV         funcOfChanged;              // ��� ������� ���������� ����� ��������� ��������� �������� ����������.

    void Draw(int x, int y, bool opened) const;
private:
    void DrawClosed(int x, int y) const;
};


class  GovernorColor
{
public:
    COMMON_PART_MENU_ITEM
    ColorType*      colorType;                  // ��������� ��� �������� �����.
    void Draw(int x, int y, bool opened);
private:
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
};


class Choice
{
public:
    COMMON_PART_MENU_ITEM
    const char *names[MAX_NUM_SUBITEMS_IN_CHOICE][2];   // �������� ������ �� ������� � ���������� ������.
    int8*       cell;                                   // ����� ������, � ������� �������� ������� �������� ������.
    pFuncVB	    funcOnChanged;                          // ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII    funcForDraw;                            // ������� ���������� ����� ��������� ��������. 
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
};


#define iEXIT   0
#define iDAY    1
#define iMONTH  2
#define iYEAR   3
#define iHOURS  4
#define iMIN    5
#define iSEC    6
#define iSET    7

// ������������� � ���������� �����.
struct Time
{
    COMMON_PART_MENU_ITEM
    int8*       curField;       // ������� ���� ���������. 0 - �����, 1 - ���, 2 - ���, 3 - ����, 4 - ����, 5 - �����, 6 - ���, 7 - ����������.
    int8*       hours;
    int8*       minutes;
    int8*       seconds;
    int8*       month;
    int8*       day;
    int8*       year;

    void Draw(int x, int y, bool opened) const;

    void SetOpened();
};

