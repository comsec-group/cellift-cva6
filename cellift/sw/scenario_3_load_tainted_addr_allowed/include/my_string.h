#pragma once

#include <stdint.h>
#include <stddef.h>

void* memcpy(void* dest, const void* src, size_t len);
void* memset(void* dest, int byte, size_t len);
size_t strlen(const char *s);
int strcmp(const char* s1, const char* s2);
int memcmp(const void* s1, const void* s2, size_t n);
char* strcpy(char* dest, const char* src);
long atol(const char* str);
