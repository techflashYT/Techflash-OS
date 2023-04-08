#include <stdint.h>

typedef struct {
	// System type: BIOS / UEFI
	uint_fast8_t systemType;
	uint_fast8_t bootbootProtocol;
} platform_t;
extern platform_t platform;