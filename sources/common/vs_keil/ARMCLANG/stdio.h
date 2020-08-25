#pragma once
#include <stdarg.h>


int vsprintf(char *__restrict, const char *__restrict, va_list);
int sprintf(char *__restrict, const char *__restrict, ...);
