#pragma once
#include "common/Keyboard/Controls_c.h"
#include "MenuItems.h"


class Menu
{
public:
    static void Draw();
    // ������� ������ ���������� � ������� �����.
    static void UpdateInput();
    // ���������� ����� �������� ����, ���������������� ������ button.
    static Item *ItemUnderButton(Key::E button);
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
    static Item *ItemUnderKey();
    // ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    static char *StringNavigation(char buffer[100]);

    static void OpenItemTime();
    // ���������� ����� ��������� �������� ����.
    static Item *OpenedItem();
    // ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������.
    static Item *CurrentItem();
    // ���������� true, ���� ������� ������� �������� � ������ namePage ������.
    static bool CurrentItemIsOpened(NamePage::E namePage);

    static NamePage::E GetNameOpenedPage();
    // ���������� ������� ������� �������� �������� �� ������ page �� ������. ���� ������� ����������� 0, ��� ����� 0, ���� ������� ����������� 1, ��� ����� 5 � �.�.
    static int PosItemOnTop(const Page *page);
    // ���������� true, ���� button - �������������� ������� [1..5].
    static bool IsFunctionalButton(Key::E button);
    // ������� �������� ������� ����.
    static void CloseOpenedItem();
    // ����� �������� ����� �������� ����, ���������������� �������������� ������� [1..5], ���� ��� ��������� � ������ ���������, � 0, ���� �� ���� ������ �� ������.
    static Item *itemUnderKey;

    static Item *itemUnderButton[Key::Count];
    // ���������� ������� ��������� ������ �� �������� namePage.
    static int GetPosActItem(NamePage::E namePage);
    // ���������� ������� ��������� ������ �� �������� namePage.
    static void SetPosActItem(NamePage::E namePage, int8 pos);
    // ���������� �� ���� �� ������.
    static bool IsShown();
    // ����������/������ ����.
    static void Show(bool show);
    // ���� true - ���� ��������� � �������������� ������.
    static bool IsMinimize();
    // ��������� ����� ��������� �� ������� �������� ����� ������.
    static void CurrentPageSBregSet(int angle);

    static bool showHelpHints;    // ���� 1, �� ��� ������� ������ ������ ���������� � ������ ��������� ��������� � � ����������
    static bool showDebugMenu;

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

    static bool NeedForFireSetLED();

    static void ResetItemsUnderButton();
   
    static void OpenFileManager();
    // ��������� ������� ������� ��������������� �������� ����.
    static void OnTimerAutoHide();
    
    static void ProcessButtonForHint(Key::E button);
};
