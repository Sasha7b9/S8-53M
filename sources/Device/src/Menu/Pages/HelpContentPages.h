#pragma once
#include "Menu/Items/MenuItems.h"
#include "Menu/Pages/HelpContent.h"


struct TypePageHelp { enum E
{
    Content,       // На этой странице расположены ссылки на другие страницы
    Description    // На этой странице нет ссылок, только описание
};};


#define MAX_PAGES 3

/** @todo Избавиться от рудимента funcNotUsed */

class PageHelpItem;

struct DataPageHelp
{
    TypePageHelp::E type;
    const PageHelpItem *parent;
    const PageHelpItem *pages[MAX_PAGES];   // Массив содержит адреса ссылаемых страниц в случае TypePageHelp::Content
};

class PageHelpItem : public Item
{
public:
    PageHelpItem(const DataItem *const data) : Item(data) {}
    const DataPageHelp *OwnData() const { return (const DataPageHelp *)(data->ad); }
    static const PageHelpItem empty;
};

DEF_PAGE_HELP(helpMenuCommon, PageHelpContent::PageMain::PageMenu::self, TypePageHelp::Description,
    "Общее описание принципов меню", "General description of the principles of the menu"
    ,
    "Кнопки на панели управления имеют два типа нажатия - короткое, длительностью менее 0.5 сек и длинное, длительностьи более 0.5 сек. "
    "Меню представляет собой древовидную структуру элементов. "
    "Главная страница меню открывается коротким либо длинным нажатием кнопки МЕНЮ. "
    "Ко всем элементам главной страницы имеется непосрдественный доступ с помощью нажатия одной из кнопок на панели управления:\n"
    "\"ДИСПЛЕЙ\" - ДИСПЛЕЙ\n"
    "\"КАНАЛ 1\" - КАНАЛ 1\n"
    "\"КАНАЛ 2\" - КАНАЛ 2\n"
    "\"СИНХР.\" - СИНХР\n"
    "\"РАЗВEРТКА\" - РАЗВ\n"
    "\"КУРСОРЫ\" - КУРСОРЫ\n"
    "\"ПАМЯТЬ\" - ПАМЯТЬ\n"
    "\"ИЗМЕРЕНИЯ\" - ИЗМЕР\n"
    "\"СЕРВИС\" - СЕРВИС\n"
    "\"ПОМОЩЬ\" - ПОМОЩЬ\n"
    "В каждом элементе главной страницы собраны настройки соответствующей категории"
    ,
    ""    
)

DEF_PAGE_HELP(helpMenuControls, PageHelpContent::PageMain::PageMenu::self, TypePageHelp::Description,
    "Описание органов управлениея", "Description of the controls"
    ,
    "",
    ""
)

DEF_PAGE_HELP(helpSCPI, PageHelpContent::PageMain::self, TypePageHelp::Description,
    "Работа с SCPI", "Working with SCPI",
    "",
    ""
)

DEF_PAGE_HELP_2(helpMenu, PageHelpContent::PageMain::self, TypePageHelp::Content,
    "Работа с меню", "Working with menus",
    "",
    "",
    helpMenuCommon,
    helpMenuControls
)

DEF_PAGE_HELP_2(pageHelpMain, &PageHelpItem::empty, TypePageHelp::Content,
    "ПОМОЩЬ", "HELP",
    "",
    "",
    helpMenu,
    helpSCPI
)


const PageHelpItem *PageHelpContent::PageMain::self = &pageHelpMain;
const PageHelpItem *PageHelpContent::PageMain::PageMenu::self = &helpMenu;
