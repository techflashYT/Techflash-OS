#include <kernel/bootloader.h>
MODULE("SMP");
extern void LM_SMP_Init();


void SMP_Init() {
	if (BOOT_LoaderID == BOOT_LoaderID_LimineCompatible) {
		LM_SMP_Init();
		return;
	}
	log("Unknown Bootloader! not initializing SMP manually, since we don't really know the machine state.", LOGLEVEL_WARN);
}