#include <external/limine.h>
#include <stdio.h>
#include <stdbool.h>
struct limine_bootloader_info_request bootloaderInfo = {
	.id = LIMINE_BOOTLOADER_INFO_REQUEST,
	.revision = 0
};


struct limine_bootloader_info_response *LM_Check() {
	// Check if we were actually booted by Limine.  Simple, see if we got a response from the "get info" request
	if (bootloaderInfo.response == NULL) {
		return NULL;
	}
	return bootloaderInfo.response;
}