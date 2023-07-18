#include <time.h>
#include <kernel/hardware/PIT.h>
clock_t clock(void) {
	return PIT_GetTicks();
}