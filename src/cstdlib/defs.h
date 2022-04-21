#include "util.h"
void *memset(void *b, int val, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
void putchar(char c);
void putc(const char c, void *stream);
uint8_t fputs(const char *str, void *stream);
__attribute__ ((format(printf, 1, 2))) uint8_t printf(const char *format, ...);
