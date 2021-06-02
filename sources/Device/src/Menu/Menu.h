// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"
#include "common/Utils/Containers/String_.h"
#include "Menu/Items/MenuItems.h"


class Menu
{
public:

    struct Handlers
    {
        // ������� ��������� ��������� ������� ������ (����� 0.5 ���.).
        static void ShortPressureKey(Key::E key);

        // ��������� �������� ������� ������.
        static void LongPressureKey(Key::E key);

        // ��������� ��������� ������ ����.
        static void PressKey(Key::E key);

        // ��������� �������� ������ �����.
        static void ReleaseKey(Key::E key);

        // ��������� �������� ����� ���������.
        static void RegulatorSet(Action::E action);

        static void KeyForHint(Key::E key);
    };

    static const int ITEMS_ON_DISPLAY = 5;     // ������� ������� ���� ���������� �� ������ �� ���������.

    static void Draw();

    // ������� ������ ���������� � ������� �����.
    static void Update();

    // ���������� ����� �������� ����, ���������������� ������ key.
    static Item *ItemUnderKey(Key::E key);

    // ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);

    // ���������� ����� ������ ����, ������������ ��� ������� � ������ ������ �������.
    static Item *ItemUnderKey();

    // ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    static Text StringNavigation();

    static void OpenItemTime();

    // ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������
    // ���������.
    static const Item *CurrentItem(bool trace = false);

    static NamePage::E GetNameOpenedPage();

    // ������� �������� ������� ����.
    static void CloseOpenedItem();

    // ������ ����
    static void Hide();

    // �������� ����
    static void Show();

    // �������� "������������" - ������ ���� ������� � ��������
    static void ChangeShowing();

    // ���������� �� ���� �� ������.
    static bool IsShown();

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

    static void OpenFileManager();

    // ��������� ������� ������� ��������������� �������� ����.
    static void OnTimerAutoHide();
};
