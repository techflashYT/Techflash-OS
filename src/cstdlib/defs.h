#include "util.hpp"
void *memset(void *b, int val, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
void putchar(char c, uint8_t x = 255, uint8_t y = 255);
void printf(const char *format, ...);
