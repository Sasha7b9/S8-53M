#pragma once

#pragma warning(push, 0)
int vsprintf(char *__restrict, const char *__restrict, struct va_list);
#pragma warning(pop)
int sprintf(char *__restrict, const char *__restrict, ...);
int snprintf(char *__restrict, unsigned int, const char *__restrict, ...);
int printf(const char *__restrict, ...);
