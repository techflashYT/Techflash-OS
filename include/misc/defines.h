#define MODULE(x) static const char *MODNAME = (x)
#define errorColor   0xFFFF4444
#define warningColor 0xFFFFD866

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#define __TFOS__     1