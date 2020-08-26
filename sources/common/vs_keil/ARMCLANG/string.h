#pragma once


size_t strlen(const char *);
int strcmp(const char *, const char *);
void *memcpy(void *, const void *, size_t);
char *strcat(char *__restrict, const char *__restrict);
char *strcpy(char *__restrict, const char *__restrict);
void *memset(void *, int, size_t);
int memcmp(const void *, const void *, size_t);
