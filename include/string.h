#define __need_size_t
#include <stddef.h>
size_t strlen(const char *str);
// set `n` bytes of `s` to `c`
void *memset(void *s, int c, size_t n);
char *strcat(char* restrict destination, const char* restrict source);
char *strcpy(char* destination, const char* source);
void *memcpy(void* destination, const void* source, size_t num);
int memcmp(const void *s1, const void *s2, int len);
int strcmp(const char *str1, const char *str2);
void *memmove(void *dest, const void *src, size_t n);
const char* strstr(const char* haystack, const char* needle);