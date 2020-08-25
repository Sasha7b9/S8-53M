#pragma once

#define va_list char *

#define va_start(x, y) x = (char *)y;

void va_end(char *);
