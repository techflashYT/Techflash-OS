#define __need_size_t
#include <stddef.h>
size_t strlen(const char *str);
void *memset(void *b, int c, size_t len);
char *strcat(char* restrict destination, const char* restrict source);
char *strcpy(char* destination, const char* source);
void *memcpy(void* destination, const void* source, size_t num);
int strcmp(const char *str1, const char *str2);