#pragma once
#include "common/Keyboard/Controls_c.h"
#include "MenuItems.h"


class Menu
{
public:
    static void Draw();
    // Функция должна вызываться в главном цикле.
    static void UpdateInput();
    // Возвращает адрес элемента меню, соответствующего данной button.
    static Item *ItemUnderButton(Key::E button);
    // Функция обработки короткого нажатия кнопки (менее 0.5 сек.).
    static void ShortPressureButton(Key::E button);
    // Функция обработки длинного нажатия кнопки (более 0.5 сек.).
    static void LongPressureButton(Key::E button);
    // Функция вызывается, когда кнопка переходит из отжатого в нажатое положение.
    static void PressButton(Key::E button);
    // Функция вызывается, когда кнопка переходит из нажатого в отжатое положение.
    static void ReleaseButton(Key::E button);
    // Функция обработки поворота ручки УСТАНОВКА вправо.
    static void RotateRegSetRight();
    // Функция обработки поворота ручки УСТАНОВКА влево.
    static void RotateRegSetLeft();
    // Установить время автоматического сокрытия меню в соответствии с установками.
    static void SetAutoHide(bool active);
    // Возвращает адрес пункта меню, находящегося под нажатой в данный момент кнопкой.
    static Item *ItemUnderKey();
    // Возвращает путь к текущему пункту меню в текстовом виде, готовом к выводу на экран.
    static char *StringNavigation(char buffer[100]);

    static void OpenItemTime();
    // Возвращает адрес открытого элемента меню.
    static Item *OpenedItem();
    // Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней.
    static Item *CurrentItem();
    // Возвращает true, если текущий элемент страницы с именем namePage открыт.
    static bool CurrentItemIsOpened(NamePage::E namePage);

    static NamePage::E GetNameOpenedPage();
    // Возвращает позицию первого элемента страницы по адресу page на экране. Если текущая подстраница 0, это будет 0, если текущая подстраница 1, это будет 5 и т.д.
    static int PosItemOnTop(const Page *page);
    // Возвращает true, если button - функциональная клавиша [1..5].
    static bool IsFunctionalButton(Key::E button);
    // Закрыть открытый элемент меню.
    static void CloseOpenedItem();
    // Здесь хранится адрес элемента меню, соответствующего функциональной клавише [1..5], если она находится в нижнем положении, и 0, если ни одна кнопка не нажата.
    static Item *itemUnderKey;

    static Item *itemUnderButton[Key::Count];
    // Возвращает позицию активного пункта на странице namePage.
    static int GetPosActItem(NamePage::E namePage);
    // Установить позицию активного пункта на странице namePage.
    static void SetPosActItem(NamePage::E namePage, int8 pos);
    // Отображено ли меню на экране.
    static bool IsShown();
    // Отобразить/скрыть меню.
    static void Show(bool show);
    // Если true - меню находится в дополнительном режиме.
    static bool IsMinimize();
    // Повернуть ручку УСТАНОВКА на текущей странице малых кнопок.
    static void CurrentPageSBregSet(int angle);

    static bool showHelpHints;    // Если 1, то при нажатии кнопки вместо выполнения её фунции выводится подсказка о её назначении
    static bool showDebugMenu;

private:
    // Обработка короткого нажатия кнопки.
    static void ProcessingShortPressureButton();
    // Обработка длинного нажатия кнопки.
    static void ProcessingLongPressureButton();
    // Обработка опускания кнопки вниз.
    static void ProcessingPressButton();
    // Обработка поднятия кнопки вверх.
    static void ProcessingReleaseButton();
    // Обработка поворота ручки УСТАНОВКА.
    static void ProcessingRegulatorSet();
    // Включить/выключить светодиод ручки УСТАНОВКА, если необходимо.
    static void SwitchSetLED();

    static bool NeedForFireSetLED();

    static void ResetItemsUnderButton();
   
    static void OpenFileManager();
    // Обработка события таймера автоматического сокрытия меню.
    static void OnTimerAutoHide();
    
    static void ProcessButtonForHint(Key::E button);
};
