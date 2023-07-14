#include <stddef.h>
extern int putcAt(char c, int_fast32_t x, int_fast32_t y, int color);
extern int putsNoTerminator(const char* str, size_t len);
extern void padTo(char *src, uint_fast8_t padding);