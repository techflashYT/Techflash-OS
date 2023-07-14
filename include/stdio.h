#include <stdarg.h>
#include <stddef.h>
extern int    putchar(const int ch);
extern int       puts(const char *string);
extern int     printf(const char *format, ...)                                                              __attribute__((format(gnu_printf, (1),   (2))));
extern int    vprintf(const char *format, va_list arg)                                                      __attribute__((format(gnu_printf, ((1)), (0))));
extern int    sprintf(char *s, const char *format, ...)                                                     __attribute__((format(gnu_printf, (2),   (3))));
extern int   vsprintf(char* s, const char* format, va_list arg)                                             __attribute__((format(gnu_printf, ((2)), (0))));
extern int   snprintf(char* s, size_t count, const char* format, ...)                                       __attribute__((format(gnu_printf, (3),   (4))));
extern int  vsnprintf(char* s, size_t count, const char* format, va_list arg)                               __attribute__((format(gnu_printf, ((3)), (0))));
extern int  fctprintf(void (*out)(char c, void* extraArg), void* extraArg, const char* format, ...)         __attribute__((format(gnu_printf, (3),   (4))));
extern int vfctprintf(void (*out)(char c, void* extraArg), void* extraArg, const char* format, va_list arg) __attribute__((format(gnu_printf, ((3)), (0))));