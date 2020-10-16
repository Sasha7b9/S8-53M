#pragma once


#define DEF_SMALL_BUTTON(name, titleRU, titleEN, hintRU, hintEN, keeper, funcPress, funcDraw)           \
static const DataSmallButton dsb##name = { funcPress, funcDraw };                                       \
const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN };                                          \
static const DataItem di##name = { TypeItem::SmallButton, &keeper, nullptr, th##name, &dsb##name};      \
static const SmallButton name(&di##name);


#define DEF_PAGE_2(name, titleRU, titleEN, hintRU, hintEN,                                              \
    item1, item2, keeper, namePage, funcActive, funcPress, funcDraw, funcRegSet)   \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                    \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, nullptr};                             \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                 \
static const DataItem di##name = { TypeItem::Page, &keeper, funcActive, th##name, &dp##name};           \
static const Page name(&di##name);


#define DEF_PAGE_5(name, titleRU, titleEN, hintRU, hintEN,                                              \
    item1, item2, item3, item4, item5, keeper, namePage, funcActive, funcPress, funcDraw, funcRegSet)   \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                    \
static Item *i##name[] = {(Item*)&item1, (Item*)&item2, (Item*)&item3, (Item*)&item4, (Item*)&item5, nullptr};                             \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                 \
static const DataItem di##name = { TypeItem::Page, &keeper, funcActive, th##name, &dp##name};           \
static const Page name(&di##name);


#define DEF_CHOCIE_2(name, titleRU, titleEN, hintRU, hintEN,                                            \
    nameRu1, nameEn1, nameRu2, nameEn2,                                                                 \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                    \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nullptr, nullptr};              \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                    \
static const DataChoice dc##name = {names##name, &cell, funcChanged, funcDraw};                         \
static const DataItem di##name = { TypeItem::Choice, &keeper, funcActive, th##name, &dc##name};         \
static const Choice name(&di##name);


#define DEF_CHOCIE_3(name, titleRU, titleEN, hintRU, hintEN,                                                        \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3,                                                           \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nullptr, nullptr};        \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                \
static const DataChoice dc##name = {names##name, &cell, funcChanged, funcDraw};                                     \
static const DataItem di##name = { TypeItem::Choice, &keeper, funcActive, th##name, &dc##name};                     \
static const Choice name(&di##name);


#define DEF_CHOCIE_7(name, titleRU, titleEN, hintRU, hintEN,                                                                                                                            \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7,                                                       \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                                                    \
static const char *names##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nullptr, nullptr};    \
static const char *th##name[4] = {titleRU, titleEN, hintRU, hintEN};                                                                                                                    \
static const DataChoice dc##name = {names##name, &cell, funcChanged, funcDraw};                                                                                                         \
static const DataItem di##name = { TypeItem::Choice, &keeper, funcActive, th##name, &dc##name};                                                                                         \
static const Choice name(&di##name);
