#define MODULE(x) static const char *MODNAME = (x)
#define errorColor   0xFFFF4444
#define warningColor 0xFFFFD866

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#define __TFOS__     1


#define KERN_VERSION_MAJOR 1
#define KERN_VERSION_MINOR 0
#define KERN_VERSION_PATCH 0

#define KERN_VERSION_MAJOR 1
#define KERN_VERSION_MINOR 0
#define KERN_VERSION_PATCH 0

// #define VER_STR KERN_VERSION_MAJOR##.##KERN_VERSION_MINOR##.##KERN_VERSION_PATCH##GIT_STR

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define VER_STR TOSTRING(KERN_VERSION_MAJOR) "." TOSTRING(KERN_VERSION_MINOR) "." TOSTRING(KERN_VERSION_PATCH) "-" TOSTRING(GIT_STR)
