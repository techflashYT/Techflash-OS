// Get all of the various types
#define __need_wchar_t
#define __need_size_t
#define __need_ptrdiff_t
#define __need_wint_t
#define __need_NULL
#include <stddef.h>
#include <stdint.h>
char *itoa(int_fast64_t value, char* result, uint_fast8_t base);
char *utoa(uint_fast64_t value, volatile char* result, uint_fast8_t base);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
int atoi(const char *nptr);
