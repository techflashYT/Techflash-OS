#include <stdarg.h>
#include <stddef.h>
#ifdef __GNUC__
#	if ((__GNUC__ == 4 && __GNUC_MINOR__>= 4) || __GNUC__ > 4)
#		define ATTR_PRINTF(one_based_format_index, first_arg) __attribute__((format(gnu_printf, (one_based_format_index), (first_arg))))
#	else
#		define ATTR_PRINTF(one_based_format_index, first_arg) __attribute__((format(printf, (one_based_format_index), (first_arg))))
#	endif
#	define ATTR_VPRINTF(one_based_format_index) ATTR_PRINTF((one_based_format_index), 0)
#else
#	define ATTR_PRINTF(one_based_format_index, first_arg)
#	define ATTR_VPRINTF(one_based_format_index)
#endif

extern int    putchar(const int ch);
extern int       puts(const char *string);
extern int     printf(const char *format, ...)                                                              ATTR_PRINTF(1, 2);
extern int    vprintf(const char *format, va_list arg)                                                      ATTR_VPRINTF(1);
extern int    sprintf(char *s, const char *format, ...)                                                     ATTR_PRINTF(2, 3);
extern int   vsprintf(char* s, const char* format, va_list arg)                                             ATTR_VPRINTF(2);
extern int   snprintf(char* s, size_t count, const char* format, ...)                                       ATTR_PRINTF(3, 4);
extern int  vsnprintf(char* s, size_t count, const char* format, va_list arg)                               ATTR_VPRINTF(3);
extern int  fctprintf(void (*out)(char c, void* extraArg), void* extraArg, const char* format, ...)         ATTR_PRINTF(3, 4);
extern int vfctprintf(void (*out)(char c, void* extraArg), void* extraArg, const char* format, va_list arg) ATTR_VPRINTF(3);

#undef ATTR_VPRINTF
#undef ATTR_PRINTF
