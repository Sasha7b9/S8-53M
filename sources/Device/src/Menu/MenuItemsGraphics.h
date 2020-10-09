#pragma once
#include "MenuItems.h"


void ItemChoice_Draw(Choice *choice, int x, int y, bool opened);
void ItemButton_Draw(Button *button, int x, int y);
void ItemSmallButton_Draw(SmallButton *smallButton, int x, int y);
void ItemPage_Draw(Page *page, int x, int y);
void ItemTime_Draw(Time *time, int x, int y, bool opened);
