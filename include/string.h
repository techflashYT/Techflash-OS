#define __need_size_t
#include <stddef.h>
size_t strlen(const char *str);
void *memset(void *b, int c, size_t len);
char *strcat(char* restrict destination, const char* restrict source);
void *memcpy(void* destination, const void* source, size_t num);