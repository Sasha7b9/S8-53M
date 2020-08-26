#pragma once

struct va_list
{
    char *a;
};

#define va_start(x, y) x.a = (char *)y;

void va_end(va_list);
