#pragma once


//typedef struct __va_list {
//    void *__stack;
//    void *__gr_top;
//    void *__vr_top;
//    int __gr_offs;
//    int __vr_offs;
//} va_list;

#define va_list char *

#define va_start(x, y) x = (char *)y;

void va_end(char *);
