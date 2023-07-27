#include <stdint.h>

typedef struct {
	// System type: BIOS / UEFI
	uint_fast8_t systemType;
} platform_t;
extern platform_t platform;