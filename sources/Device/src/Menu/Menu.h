// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"
#include "common/Utils/Containers/String_.h"
#include "Menu/Items/MenuItems.h"


class Menu
{
public:

    // Обработка длинного нажатия кнопки.
    static void HandlerLongPressureButton();

    // Функция обработки короткого нажатия кнопки (менее 0.5 сек.).
    static void HandlerShortPressureButton(Key::E button);

    struct Event
    {
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
    static void Update();

    // Возвращает адрес элемента меню, соответствующего данной button.
    static Item *ItemUnderButton(Key::E button);

    // Установить время автоматического сокрытия меню в соответствии с установками.
    static void SetAutoHide(bool active);

    // Возвращает адрес пункта меню, находящегося под нажатой в данный момент кнопкой.
    static Item *ItemUnderKey();

    // Возвращает путь к текущему пункту меню в текстовом виде, готовом к выводу на экран.
    static Text StringNavigation();

    static void OpenItemTime();

    // Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата
    // последней.
    static const Item *CurrentItem();
    static bool IsCurrentItem(const Item *item);

    static NamePage::E GetNameOpenedPage();

    // Закрыть открытый элемент меню.
    static void CloseOpenedItem();

    // Скрыть меню
    static void Hide();

    // Показать меню
    static void Show();

    // Изменить "показанность" - скрыть если открыто и наоборот
    static void ChangeShowing();

    // Отображено ли меню на экране.
    static bool IsShown();

    // Если true - меню находится в дополнительном режиме.
    static bool IsMinimize();

    // Повернуть ручку УСТАНОВКА на текущей странице малых кнопок.
    static void CurrentPageSBregSet(int angle);

    static void DrawStringNavigation();

    static bool showHelpHints;      // Если 1, то при нажатии кнопки вместо выполнения её фунции выводится подсказка о
                                    // её назначении
    static bool showDebugMenu;
    static bool needClosePageSB;    // Если 1, нужно закрывать страницу малых кнопок

private:

    // Обработка опускания кнопки вниз.
    static void ProcessingPressButton();

    // Обработка поднятия кнопки вверх.
    static void ProcessingReleaseButton();

    // Обработка поворота ручки УСТАНОВКА.
    static void ProcessingRegulatorSet();

    static void OpenFileManager();

    // Обработка события таймера автоматического сокрытия меню.
    static void OnTimerAutoHide();
    
    static void ProcessButtonForHint(Key::E button);
};
