#pragma once
#include "common/Keyboard/Controls_c.h"
#include "MenuItems.h"


class Menu
{
public:
    // ���������� ����� �������� ����, ���������������� ������ button.
    static void* ItemUnderButton(Key::E button);
    // ���������� true, ���� ������� ���� item ������ (��������� �� �� ����� ������� ����. ��� �������, ��� ��������, ��� ������� �������������� ������� ���� ����� Choice ��� Governor.
    static bool IsShade(const void *item);

    static bool IsPressed(const void* item);

    static void Draw();
    // ������� ������ ���������� � ������� �����.
    static void UpdateInput();
    // ������� ��������� ��������� ������� ������ (����� 0.5 ���.).
    static void ShortPressureButton(Key::E button);
    // ������� ��������� �������� ������� ������ (����� 0.5 ���.).
    static void LongPressureButton(Key::E button);
    // ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
    static void PressButton(Key::E button);
    // ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
    static void ReleaseButton(Key::E button);
    // ������� ��������� �������� ����� ��������� ������.
    static void RotateRegSetRight();
    // ������� ��������� �������� ����� ��������� �����.
    static void RotateRegSetLeft();
    // ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);
    // ���������� ����� ������ ����, ������������ ��� ������� � ������ ������ �������.
    static void *ItemUnderKey();
    // ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    static char *StringNavigation(char buffer[100]);

    static void OpenItemTime();
    // ���������� true, ���� ������� ���� item �������� �������, �.�. ����� ���� �����.
    static bool ItemIsActive(const void *item);
    // ���������� ���������� ��������� � �������� �� ������ page.
    static int NumItemsInPage(const Page * const page);
    // ���������� ����� ������� ���������� �������� �� ������ page.
    static int NumCurrentSubPage(const Page *page);
    // D��������� ����� ���������� � �������� �� ������ page.
    static int NumSubPages(const Page *page);
    // ���������� ��� �������� ���� �� ������ address.
    static TypeItem::E TypeMenuItem(const void *address);
    // ���������� ��� ��������� �������� ����.
    static TypeItem::E TypeOpenedItem();
    // ���������� ����� ��������� �������� ����.
    static void *OpenedItem();
    // ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������.
    static void *CurrentItem();
    // ���������� ����� �������� ���� �������� ��������.
    static void *GetItem(const Page *page, int numElement);
    // ���������� true, ���� ������� ������� �������� � ������ namePage ������.
    static bool CurrentItemIsOpened(NamePage::E namePage);
    // ���������� ������� �������� �������� ������� page.
    static int8 PosCurrentItem(const Page *page);
    // �������/��������� ������� ����� ��������.
    static void SetCurrentItem(const void *item, bool active);
    // ���������� ����� ��������, �������� ����������� ������� �� ������ item.
    static Page* Keeper(const void *item);

    static NamePage::E GetNameOpenedPage();

    static void OpenPageAndSetItCurrent(void *page);

    static bool ChangeOpenedItem(void *item, int delta);
    // ��������� ��� ����������� �������� Governor, GovernorColor ��� Choice �� ������ item � ����������� �� ����� delta
    static void ChangeItem(void *item, int delta);
    // ���������� ������ � �������� ��������� �������� Choice ��� NamePage::E �� ������ item.
    static int HeightOpenedItem(const void *item);
    // ���������� �������� �������� �� ������ item, ��� ��� �������� �� ������� �������.
    static const char* TitleItem(const void *item);
    // ���������� ������� ������� �������� �������� �� ������ page �� ������. ���� ������� ����������� 0, ��� ����� 0, ���� ������� ����������� 1, ��� ����� 5 � �.�.
    static int PosItemOnTop(const Page *page);
    // ���������� true, ���� button - �������������� ������� [1..5].
    static bool IsFunctionalButton(Key::E button);
    // ������� �������� ������� ����.
    static void CloseOpenedItem();

    static void OpenItem(const void *item, bool open);          // �������/������� ������� ���� �� ������ item.

    static bool ItemIsOpened(const void *item);                       // ���������� true, ���� ������� ���� �� ������ item ������.

    static void ChangeSubPage(const Page *page, int delta);

    static void ShortPressOnPageItem(Page *page, int numItem);   // �������� ������� ��������� ������� ������ ��� ������ numItem �������� page;

    static Page *PagePointerFromName(NamePage::E namePage);

    static bool PageIsSB(const Page *page);

    static SmallButton *SmallButonFromPage(Page *page, int numButton);

    static void ChangeStateFlashDrive();

private:
    // ��������� ��������� ������� ������.
    static void ProcessingShortPressureButton();
    // ��������� �������� ������� ������.
    static void ProcessingLongPressureButton();
    // ��������� ��������� ������ ����.
    static void ProcessingPressButton();
    // ��������� �������� ������ �����.
    static void ProcessingReleaseButton();
    // ��������� �������� ����� ���������.
    static void ProcessingRegulatorSet();
    // ��������/��������� ��������� ����� ���������, ���� ����������.
    static void SwitchSetLED();
    // ��������� ��������� ������� �� ������� NamePage::E � ������� page.
    static void ShortPress_Page(void *page);
    // ��������� ��������� ������� �� ������� Choice � ������� choice.
    static void ShortPress_Choice(void *choice);

    static void ShortPress_Time(void *time);
    // ��������� ��������� ������� �� ������� Button � ������� button.
    static void ShortPress_Button(void *button);
    // ��������� ��������� ������� �� ������� GovernorColor � ������� governorColor.
    static void ShortPress_GovernorColor(void *governorColor);

    static void ShortPress_IP(void *item);

    static void ShortPress_MAC(void *item);

    static void ShortPress_ChoiceReg(void *choice);

    static void ShortPress_SmallButton(void *smallButton);
    // ��������� �������� ������� �� ������� ���� item.
    static void FuncOnLongPressItem(void *item);

    static void FuncOnLongPressItemTime(void *item);
    // ��������� �������� ������� �� ������� Button � ������� button.
    static void FuncOnLongPressItemButton(void *button);
    // ���������� ������� ��������� ��������� ������� �� ������� ���� item.
    static void ExecuteFuncForShortPressOnItem(void *item);
    // ���������� ������� ��������� �������� ������� �� ������� ���� item.
    static void ExecuteFuncForLongPressureOnItem(void *item);

    static bool NeedForFireSetLED();

    static void DrawOpenedPage(Page *page, int layer, int yTop);

    static void DrawTitlePage(Page *page, int layer, int yTop);

    static void DrawItemsPage(const Page *page, int layer, int yTop);

    static void DrawPagesUGO(const Page *page, int right, int bottom);

    static int CalculateX(int layer);

    static void ResetItemsUnderButton();

    static int ItemOpenedPosY(const void *item);

    static void *RetLastOpened(Page *_page, TypeItem::E *_type);
    
    static void OpenFileManager();
    // ��������� ������� ������� ��������������� �������� ����.
    static void OnTimerAutoHide();
    
    static void ProcessButtonForHint(Key::E button);
};
