// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"
#include "common/Utils/Containers/String_.h"
#include "Menu/Items/MenuItems.h"


class Menu
{
public:

    struct Event
    {
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
    };

    static const int ITEMS_ON_DISPLAY = 5;     // ������� ������� ���� ���������� �� ������ �� ���������.

    static void Draw();

    // ������� ������ ���������� � ������� �����.
    static void Update();

    // ���������� ����� �������� ����, ���������������� ������ button.
    static Item *ItemUnderButton(Key::E button);

    // ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);

    // ���������� ����� ������ ����, ������������ ��� ������� � ������ ������ �������.
    static Item *ItemUnderKey();

    // ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    static Text StringNavigation();

    static void OpenItemTime();

    // ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������
    // ���������.
    static const Item *CurrentItem();
    static bool IsCurrentItem(const Item *item);

    static NamePage::E GetNameOpenedPage();

    // ������� �������� ������� ����.
    static void CloseOpenedItem();

    // ���������� �� ���� �� ������.
    static bool IsShown();

    // ����������/������ ����.
    static void Show(bool show);

    static void Hide();

    // ���� true - ���� ��������� � �������������� ������.
    static bool IsMinimize();

    // ��������� ����� ��������� �� ������� �������� ����� ������.
    static void CurrentPageSBregSet(int angle);

    static void DrawStringNavigation();

    static bool showHelpHints;      // ���� 1, �� ��� ������� ������ ������ ���������� � ������ ��������� ��������� �
                                    // � ����������
    static bool showDebugMenu;
    static bool needClosePageSB;    // ���� 1, ����� ��������� �������� ����� ������

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

    static void OpenFileManager();

    // ��������� ������� ������� ��������������� �������� ����.
    static void OnTimerAutoHide();
    
    static void ProcessButtonForHint(Key::E button);
};
