#include <stdint.h>

typedef struct {
	// System type: BIOS / UEFI
	uint8_t systemType;
	uint8_t bootbootProtocol;
} platform_t;
extern platform_t platform;