#pragma once
#include "common/Keyboard/Controls_.h"
#include "Menu/Items/MenuItems.h"


class Menu
{
public:

    struct Event
    {
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
    };

    static const int ITEMS_ON_DISPLAY = 5;     // Сколько пунктов меню помещается на экране по вертикали.

    static void Draw();

    // Функция должна вызываться в главном цикле.
    static void UpdateInput();

    // Возвращает адрес элемента меню, соответствующего данной button.
    static Item *ItemUnderButton(Key::E button);

    // Установить время автоматического сокрытия меню в соответствии с установками.
    static void SetAutoHide(bool active);

    // Возвращает адрес пункта меню, находящегося под нажатой в данный момент кнопкой.
    static Item *ItemUnderKey();

    // Возвращает путь к текущему пункту меню в текстовом виде, готовом к выводу на экран.
    static char *StringNavigation(char buffer[100]);

    static void OpenItemTime();

    // Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней.
    static const Item *CurrentItem();
    static bool IsCurrentItem(const Item *item);

    static NamePage::E GetNameOpenedPage();

    // Закрыть открытый элемент меню.
    static void CloseOpenedItem();

    // Отображено ли меню на экране.
    static bool IsShown();

    // Отобразить/скрыть меню.
    static void Show(bool show);

    // Если true - меню находится в дополнительном режиме.
    static bool IsMinimize();

    // Повернуть ручку УСТАНОВКА на текущей странице малых кнопок.
    static void CurrentPageSBregSet(int angle);

    static bool showHelpHints;      // Если 1, то при нажатии кнопки вместо выполнения её фунции выводится подсказка о её назначении
    static bool showDebugMenu;
    static bool needClosePageSB;    // Если 1, нужно закрывать страницу малых кнопок

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

    static void OpenFileManager();

    // Обработка события таймера автоматического сокрытия меню.
    static void OnTimerAutoHide();
    
    static void ProcessButtonForHint(Key::E button);
};
