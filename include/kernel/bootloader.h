#include <stdint.h>
extern uint_fast8_t BOOT_LoaderID;
extern char BOOT_LoaderName[64];
extern char BOOT_LoaderVersion[64];

#define BOOT_LoaderID_Unknown 0
#define BOOT_LoaderID_LimineCompatible 1


extern void BOOT_CheckLoader();