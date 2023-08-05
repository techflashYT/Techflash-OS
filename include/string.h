#define __need_size_t
#include <stddef.h>
#include <stdint.h>
// set `n` bytes of `s` to `c`
extern void   *memset(void *s, uint8_t c, size_t n);
extern void   *memcpy(void *destination, const void *source, size_t num);
extern int     memcmp(const void *s1, const void *s2, size_t len);
extern void   *memmove(void *dest, const void *src, size_t n);
extern int     strcmp(const char *str1, const char *str2);
extern char   *strcat(char *restrict destination, const char *restrict source);
extern char   *strcpy(char *destination, const char *source);
extern char   *strstr(const char *haystack, const char *needle);
extern size_t  strnlen_s(const char *str, size_t strsz);
extern size_t  strlen(const char *str);
