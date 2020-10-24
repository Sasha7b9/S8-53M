#pragma once


#define DEF_BUTTON(name, keeper, titleRU, titleEN, hintRU, hintEN, funcActive, funcPress)                                                   \
static const DataButton db##name = { funcPress };                                                                                           \
static const char *th##name[] = {titleRU, titleEN, hintRU, hintEN};                                                                         \
static const DataItem di##name = { TypeItem::Button, keeper, funcActive, th##name, &db##name};                                              \
static const Button name(&di##name);


#define DEF_SMALL_BUTTON(name, keeper, titleRU, titleEN, hintRU, hintEN, funcActive, funcPress, funcDraw, hintUGO)                          \
static const DataSmallButton dsb##name = { funcPress, funcDraw, hintUGO };                                                                  \
const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN };                                                                              \
static const DataItem di##name = { TypeItem::SmallButton, keeper, funcActive, th##name, &dsb##name};                                        \
static const SmallButton name(&di##name);


#define DEF_GOVERNOR(name, keeper, titleRU, titleEN, hintRU, hintEN, cell, min, max, funcActive, funcChange, funcBeforeDraw)                \
static const DataGovernor dg##name = { &cell, min, max, funcChange, funcBeforeDraw };                                                       \
static const char *th##name[] = {titleRU, titleEN, hintRU, hintEN};                                                                         \
static const DataItem di##name = { TypeItem::Governor, keeper, funcActive, th##name, &dg##name};                                            \
static const Governor name(&di##name);


#define DEF_GOVERNOR_COLOR(name, keeper, titleRU, titleEN, hintRU, hintEN, colorType, funcActive)                                           \
static const DataGovernorColor dgc##name = { &colorType };                                                                                  \
static const char *th##name[] = {titleRU, titleEN, hintRU, hintEN};                                                                         \
static const DataItem di##name = { TypeItem::GovernorColor, keeper, funcActive, th##name, &dgc##name};                                      \
static const GovernorColor name(&di##name);


#define DEF_IPADDRESS(name, keeper, titleRU, titleEN, hintRU, hintEN, ip0, ip1, ip2, ip3, port, funcOnChanged)                              \
static const DataIPaddress dip##name = {&ip0, &ip1, &ip2, &ip3, funcOnChanged, &port};                                                      \
static const char *th##name[] = {titleRU, titleEN, hintRU, hintEN};                                                                         \
static const DataItem di##name = { TypeItem::IP, keeper, nullptr, th##name, &dip##name};                                                    \
static const IPaddress name(&di##name);


#define DEF_MACADDRESS(name, keeper, titleRU, titleEN, hintRU, hintEN, mac0, mac1, mac2, mac3, mac4, mac5, funcOnChanged)                   \
static const DataMACaddress dmac##name = {&mac0, &mac1, &mac2, &mac3, &mac4, &mac5, funcOnChanged};                                         \
static const char *th##name[] = {titleRU, titleEN, hintRU, hintEN};                                                                         \
static const DataItem di##name = { TypeItem::MAC, keeper, nullptr, th##name, &dmac##name};                                                  \
static const MACaddress name(&di##name);


#define DEF_TIME(name, keeper, titleRU, titleEN, hintRU, hintEN, curField, hours, minutes, seconds, month, day, year)                       \
static const DataTime dt##name = {&curField, &hours, &minutes, &seconds, &month, &day, &year};                                              \
static const char *th##name[] = {titleRU, titleEN, hintRU, hintEN};                                                                         \
static const DataItem di##name = { TypeItem::Time, (const Page *)keeper, nullptr, th##name, &dt##name};                                     \
static const TimeItem name(&di##name);


#define DEF_PAGE_HELP(name, parent, type, titleRU, titleEN, hintRU, hintEN)                                                                 \
static const DataPageHelp dph##name = { type, parent, nullptr };                                                                            \
static const char *th##name[] = {titleRU, titleEN, hintRU, hintEN};                                                                         \
static const DataItem di##name = { TypeItem::Page, nullptr, nullptr, th##name, &dph##name};                                                 \
static const PageHelpItem name(&di##name);


#define DEF_PAGE_HELP_2(name, parent, type, titleRU, titleEN, hintRU, hintEN, page1, page2)                                                 \
static const DataPageHelp dph##name = { type, parent, {&page1, &page2} };                                                                   \
static const char *th##name[] = {titleRU, titleEN, hintRU, hintEN};                                                                         \
static const DataItem di##name = { TypeItem::Page, nullptr, nullptr, th##name, &dph##name};                                                 \
static const PageHelpItem name(&di##name);


#define DEF_PAGE_2(name, keeper, namePage, titleRU, titleEN, hintRU, hintEN, item1, item2, funcActive, funcPress, funcDraw, funcRegSet)     \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                        \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, nullptr};                                                                           \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                      \
static const DataItem di##name = { TypeItem::Page, keeper, funcActive, th##name, &dp##name};                                                \
static int8 subPage##name;                                                                                                                  \
static const Page name(&di##name, &subPage##name);


#define DEF_PAGE_3(name, keeper, namePage, titleRU, titleEN, hintRU, hintEN, item1, item2, item3, funcActive, funcPress, funcDraw, funcRegSet)      \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                \
static Item *i##name[] = {(Item *)&item1, (Item *)&item2, (Item *)&item3, nullptr};                                                                 \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                              \
static const DataItem di##name = { TypeItem::Page, keeper, funcActive, th##name, &dp##name};                                                        \
static int8 subPage##name;                                                                                                                          \
static const Page name(&di##name, &subPage##name);


#define DEF_PAGE_4(name, keeper, namePage, titleRU, titleEN, hintRU, hintEN,                                                                        \
    item1, item2, item3, item4, funcActive, funcPress, funcDraw, funcRegSet)                                                                        \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, (Item*)&item3, (Item*)&item4, nullptr};                                                     \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                              \
static const DataItem di##name = { TypeItem::Page, keeper, funcActive, th##name, &dp##name};                                                        \
static int8 subPage##name;                                                                                                                          \
static const Page name(&di##name, &subPage##name);


#define DEF_PAGE_5(name, keeper, namePage, titleRU, titleEN, hintRU, hintEN,                                                                        \
    item1, item2, item3, item4, item5, funcActive, funcPress, funcDraw, funcRegSet)                                                                 \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, (Item*)&item3, (Item*)&item4, (Item*)&item5, nullptr};                                      \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                              \
static const DataItem di##name = { TypeItem::Page, keeper, funcActive, th##name, &dp##name};                                                        \
static int8 subPage##name;                                                                                                                          \
const Page name(&di##name, &subPage##name);


#define DEF_PAGE_6(name, keeper, namePage, titleRU, titleEN, hintRU, hintEN, item1, item2, item3, item4, item5, item6,                              \
    funcActive, funcPress, funcDraw, funcRegSet)                                                                                                    \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, (Item*)&item3, (Item*)&item4, (Item*)&item5, (Item*)&item6, nullptr};                       \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                              \
static const DataItem di##name = { TypeItem::Page, keeper, funcActive, th##name, &dp##name};                                                        \
static int8 subPage##name;                                                                                                                          \
static const Page name(&di##name, &subPage##name);


#define DEF_PAGE_7(name, keeper, namePage, titleRU, titleEN, hintRU, hintEN, item1, item2, item3, item4, item5, item6, item7,                       \
    funcActive, funcPress, funcDraw, funcRegSet)                                                                                                    \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, (Item*)&item3, (Item*)&item4, (Item*)&item5, (Item*)&item6, (Item*)&item7, nullptr};        \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                              \
static const DataItem di##name = { TypeItem::Page, keeper, funcActive, th##name, &dp##name};                                                        \
static int8 subPage##name;                                                                                                                          \
static const Page name(&di##name, &subPage##name);


#define DEF_PAGE_9(name, keeper, namePage, titleRU, titleEN, hintRU, hintEN, item1, item2, item3, item4, item5, item6, item7, item8, item9,                                     \
    funcActive, funcPress, funcDraw, funcRegSet)                                                                                                                                \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                            \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, (Item*)&item3, (Item*)&item4, (Item*)&item5, (Item*)&item6, (Item*)&item7, (Item*)&item8, (Item*)&item9, nullptr};      \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                          \
static const DataItem di##name = { TypeItem::Page, keeper, funcActive, th##name, &dp##name};                                                                                    \
static int8 subPage##name;                                                                                                                                                      \
static const Page name(&di##name, &subPage##name);


#define DEF_PAGE_10(name, keeper, namePage, titleRU, titleEN, hintRU, hintEN,                                                                                                               \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10,     funcActive, funcPress, funcDraw, funcRegSet)                                                                 \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                                        \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, (Item*)&item3, (Item*)&item4, (Item*)&item5, (Item*)&item6, (Item*)&item7, (Item*)&item8, (Item*)&item9, (Item*)&item10, nullptr};  \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { TypeItem::Page, keeper, funcActive, th##name, &dp##name};                                                                                                \
static int8 subPage##name;                                                                                                                                                                  \
static const Page name(&di##name, &subPage##name);


#define DEF_PAGE_11(name, keeper, namePage, titleRU, titleEN, hintRU, hintEN,                                                                                                               \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11,funcActive, funcPress, funcDraw, funcRegSet)                                                              \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                                        \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, (Item*)&item3, (Item*)&item4, (Item*)&item5, (Item*)&item6,                                                                         \
                            (Item*)&item7, (Item*)&item8, (Item*)&item9, (Item*)&item10, (Item*)&item11, nullptr};                                                                          \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { TypeItem::Page, &keeper, funcActive, th##name, &dp##name};                                                                                               \
static int8 subPage##name;                                                                                                                                                                  \
static const Page name(&di##name, &subPage##name);


#define DEF_PAGE_12(name, keeper, namePage, titleRU, titleEN, hintRU, hintEN,                                                                                                               \
                    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, funcActive, funcPress, funcDraw, funcRegSet)                                     \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                                        \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, (Item*)&item3, (Item*)&item4, (Item*)&item5, (Item*)&item6,                                                                         \
                            (Item*)&item7, (Item*)&item8, (Item*)&item9, (Item*)&item10, (Item*)&item11, (Item*)&item12, nullptr};                                                          \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { TypeItem::Page, keeper, funcActive, th##name, &dp##name};                                                                                                \
static int8 subPage##name;                                                                                                                                                                  \
static const Page name(&di##name, &subPage##name);


#define DEF_CHOICE_2(name, keeper, titleRU, titleEN, hintRU, hintEN, nameRu1, nameEn1, nameRu2, nameEn2, cell, funcActive, funcChanged, funcDraw)   \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nullptr};                                                                   \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                \
static const DataChoice dc##name = {names##name, (int8 *)(&cell), funcChanged, funcDraw};                                                           \
static const DataItem di##name = { TypeItem::Choice, keeper, funcActive, th##name, &dc##name};                                                      \
static const Choice name(&di##name);


#define DEF_CHOICE_3(name, keeper, titleRU, titleEN, hintRU, hintEN, nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, cell, funcActive, funcChanged, funcDraw) \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nullptr};                                                                 \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                                             \
static const DataItem di##name = { TypeItem::Choice, keeper, funcActive, th##name, &dc##name};                                                                      \
static const Choice name(&di##name);


#define DEF_CHOICE_4(name, keeper, titleRU, titleEN, hintRU, hintEN,                                                                    \
                    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, cell, funcActive, funcChanged, funcDraw)    \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nullptr};                   \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                    \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                 \
static const DataItem di##name = { TypeItem::Choice, keeper, funcActive, th##name, &dc##name};                                          \
static const Choice name(&di##name);


#define DEF_CHOICE_5(name, keeper, titleRU, titleEN, hintRU, hintEN,                                                                        \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, cell, funcActive, funcChanged, funcDraw)      \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nullptr};     \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                        \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                     \
static const DataItem di##name = { TypeItem::Choice, keeper, funcActive, th##name, &dc##name};                                              \
static const Choice name(&di##name);


#define DEF_CHOICE_6(name, keeper, titleRU, titleEN, hintRU, hintEN,                                                                                        \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, cell, funcActive, funcChanged, funcDraw)    \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nullptr};   \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                        \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                                     \
static const DataItem di##name = { TypeItem::Choice, keeper, funcActive, th##name, &dc##name};                                                              \
static const Choice name(&di##name);


#define DEF_CHOICE_7(name, keeper, titleRU, titleEN, hintRU, hintEN,                                                                                                            \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, cell, funcActive, funcChanged, funcDraw)      \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nullptr};     \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                            \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                                                         \
static const DataItem di##name = { TypeItem::Choice, keeper, funcActive, th##name, &dc##name};                                                                                  \
static const Choice name(&di##name);


#define DEF_CHOICE_8(name, keeper, titleRU, titleEN, hintRU, hintEN,                                                                                                                        \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, cell, funcActive, funcChanged, funcDraw)\
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nullptr};\
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                                        \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                                                                     \
static const DataItem di##name = { TypeItem::Choice, &keeper, funcActive, th##name, &dc##name};                                                                                             \
static const Choice name(&di##name);


#define DEF_CHOICE_REG_8(name, keeper, titleRU, titleEN, hintRU, hintEN, nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4,                             \
                        nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, cell, funcActive, funcChanged, funcDraw)                   \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4,                                                                  \
                                    nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nullptr};                                      \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                                             \
static const DataItem di##name = { TypeItem::ChoiceReg, keeper, funcActive, th##name, &dc##name};                                                                   \
static const Choice name(&di##name);


#define DEF_CHOICE_9(name, keeper, titleRU, titleEN, hintRU, hintEN, nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4,                                 \
                    nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, cell, funcActive, funcChanged, funcDraw)     \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7,   \
                                    nameRu8, nameEn8, nameRu9, nameEn9, nullptr};                                                                                   \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                                             \
static const DataItem di##name = { TypeItem::Choice, &keeper, funcActive, th##name, &dc##name};                                                                     \
static const Choice name(&di##name);


#define DEF_CHOICE_REG_9(name, keeper, titleRU, titleEN, hintRU, hintEN,                                                                                                                    \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9,                       \
    cell, funcActive, funcChanged, funcDraw)                                                                                                                                                \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8,         \
                                    nameRu9, nameEn9, nullptr};                                                                                                                             \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                                        \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                                                                     \
static const DataItem di##name = { TypeItem::ChoiceReg, keeper, funcActive, th##name, &dc##name};                                                                                           \
static const Choice name(&di##name);


#define DEF_CHOICE_10(name, keeper, titleRU, titleEN, hintRU, hintEN,                                                                                                                       \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10,   \
    cell, funcActive, funcChanged, funcDraw)                                                                                                                                                \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9,\
                                    nameEn9, nameRu10, nameEn10, nullptr};                                                                                                                  \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                                        \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                                                                     \
static const DataItem di##name = { TypeItem::Choice, &keeper, funcActive, th##name, &dc##name};                                                                                             \
static const Choice name(&di##name);


#define DEF_CHOICE_REG_10(name, keeper, titleRU, titleEN, hintRU, hintEN,                                                                                                                   \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10,   \
    cell, funcActive, funcChanged, funcDraw)                                                                                                                                                \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9,\
                                    nameEn9, nameRu10, nameEn10, nullptr};                                                                                                                  \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                                        \
static const DataChoice dc##name = {names##name, (int8 *)&cell, funcChanged, funcDraw};                                                                                                     \
static const DataItem di##name = { TypeItem::ChoiceReg, keeper, funcActive, th##name, &dc##name};                                                                                           \
static const Choice name(&di##name);
