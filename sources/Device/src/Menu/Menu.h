#pragma once
#include "common/Keyboard/Controls_c.h"
#include "MenuItems.h"


class Menu
{
public:
    // Возвращает адрес элемента меню, соответствующего данной button.
    static void* ItemUnderButton(Key::E button);
    // Возвращает true, если элемент меню item затенён (находится не на самом верхнем слое. Как правило, это означает, что раскрыт раскрывающийся элемент меню вроде Choice или Governor.
    static bool IsShade(const void *item);

    static bool IsPressed(const void* item);

    static void Draw();
    // Функция должна вызываться в главном цикле.
    static void UpdateInput();
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
    static void *ItemUnderKey();
    // Возвращает путь к текущему пункту меню в текстовом виде, готовом к выводу на экран.
    static char *StringNavigation(char buffer[100]);

    static void OpenItemTime();
    // Вовзращает true, если элемент меню item является ативным, т.е. может быть нажат.
    static bool ItemIsActive(const void *item);
    // Возвращает количество элементов в странице по адресу page.
    static int NumItemsInPage(const Page * const page);
    // Возвращает номер текущей подстранцы элемента по адресу page.
    static int NumCurrentSubPage(const Page *page);
    // Dозвращает число подстраниц в странице по адресу page.
    static int NumSubPages(const Page *page);
    // Возвращает тип элемента меню по адресу address.
    static TypeItem::E TypeMenuItem(const void *address);
    // Возвращает тип открытого элемента меню.
    static TypeItem::E TypeOpenedItem();
    // Возвращает адрес открытого элемента меню.
    static void *OpenedItem();
    // Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней.
    static void *CurrentItem();
    // Возвращает адрес элемента меню заданной страницы.
    static void *GetItem(const Page *page, int numElement);
    // Возвращает true, если текущий элемент страницы с именем namePage открыт.
    static bool CurrentItemIsOpened(NamePage::E namePage);
    // Возвращает позицию текущего элемента странцы page.
    static int8 PosCurrentItem(const Page *page);
    // Сделать/разделать текущим пункт страницы.
    static void SetCurrentItem(const void *item, bool active);
    // Возвращает адрес элемента, которому принадлежит элемент по адресу item.
    static Page* Keeper(const void *item);

    static NamePage::E GetNameOpenedPage();

    static void OpenPageAndSetItCurrent(void *page);

    static bool ChangeOpenedItem(void *item, int delta);
    // Уменьшает или увеличивает значение Governor, GovernorColor или Choice по адресу item в зависимости от знака delta
    static void ChangeItem(void *item, int delta);
    // Возвращает высоту в пикселях открытого элемента Choice или NamePage::E по адресу item.
    static int HeightOpenedItem(const void *item);
    // Возвращает название элемента по адресу item, как оно выглядит на дисплее прибора.
    static const char* TitleItem(const void *item);
    // Возвращает позицию первого элемента страницы по адресу page на экране. Если текущая подстраница 0, это будет 0, если текущая подстраница 1, это будет 5 и т.д.
    static int PosItemOnTop(const Page *page);
    // Возвращает true, если button - функциональная клавиша [1..5].
    static bool IsFunctionalButton(Key::E button);
    // Закрыть открытый элемент меню.
    static void CloseOpenedItem();

    static void OpenItem(const void *item, bool open);          // Открыть/закрыть элемент меню по адрему item.

    static bool ItemIsOpened(const void *item);                       // Возвращает true, если элемент меню по адрему item открыт.

    static void ChangeSubPage(const Page *page, int delta);

    static void ShortPressOnPageItem(Page *page, int numItem);   // Вызывает функцию короткого нажатия кнопки над итемом numItem страницы page;

    static Page *PagePointerFromName(NamePage::E namePage);

    static bool PageIsSB(const Page *page);

    static SmallButton *SmallButonFromPage(Page *page, int numButton);

    static void ChangeStateFlashDrive();

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
    // Обработка короткого нажатия на элемент NamePage::E с адресом page.
    static void ShortPress_Page(void *page);
    // Обработка короткого нажатия на элемент Choice с адресом choice.
    static void ShortPress_Choice(void *choice);

    static void ShortPress_Time(void *time);
    // Обработка короткого нажатия на элемент Button с адресом button.
    static void ShortPress_Button(void *button);
    // Обработка короткого нажатия на элемент GovernorColor с адресом governorColor.
    static void ShortPress_GovernorColor(void *governorColor);

    static void ShortPress_IP(void *item);

    static void ShortPress_MAC(void *item);

    static void ShortPress_ChoiceReg(void *choice);

    static void ShortPress_SmallButton(void *smallButton);
    // Обработка длинного нажатия на элемент меню item.
    static void FuncOnLongPressItem(void *item);

    static void FuncOnLongPressItemTime(void *item);
    // Обработка длинного нажатия на элемент Button с адресом button.
    static void FuncOnLongPressItemButton(void *button);
    // Возвращает функцию обработки короткого нажатия на элемент меню item.
    static void ExecuteFuncForShortPressOnItem(void *item);
    // Возвращает функцию обработки длинного нажатия на элемент меню item.
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
    // Обработка события таймера автоматического сокрытия меню.
    static void OnTimerAutoHide();
    
    static void ProcessButtonForHint(Key::E button);
};
