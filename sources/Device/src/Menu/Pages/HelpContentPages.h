#pragma once
#include "Menu/MenuItems.h"


struct TypePageHelp { enum E
{
    Content,       // На этой странице расположены ссылки на другие страницы
    Description    // На этой странице нет ссылок, только описание
};};


#define MAX_PAGES 3

/** @todo Избавиться от рудимента funcNotUsed */

class PageHelp;

struct DataPageHelp
{
    TypePageHelp::E type;
    const PageHelp *parent;
    const PageHelp *pages[MAX_PAGES];   // Массив содержит адреса ссылаемых страниц в случае TypePageHelp::Content
};

class PageHelp : public Item
{
public:
    PageHelp(const DataItem *const data) : Item(data) {}
    const DataPageHelp *OwnData() const { return static_cast<const DataPageHelp *>(data->ad); }
    static const PageHelp empty;
};

extern const PageHelp helpMenu;


DEF_PAGE_HELP(helpMenuCommon, helpMenu, TypePageHelp::Description,
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

DEF_PAGE_HELP(helpMenuControls, helpMenu, TypePageHelp::Description,
    "Описание органов управлениея", "Description of the controls"
    ,
    "",
    ""
)

extern const PageHelp helpMain;

DEF_PAGE_HELP(helpSCPI, helpMain, TypePageHelp::Description,
    "Работа с SCPI", "Working with SCPI",
    "",
    ""
)

DEF_PAGE_HELP_2(helpMenu, helpMain, TypePageHelp::Content,
    "Работа с меню", "Working with menus",
    "",
    "",
    helpMenuCommon,
    helpMenuControls
)

DEF_PAGE_HELP_2(helpMain, PageHelp::empty, TypePageHelp::Content,
    "ПОМОЩЬ", "HELP",
    "",
    "",
    helpMenu,
    helpSCPI
)
