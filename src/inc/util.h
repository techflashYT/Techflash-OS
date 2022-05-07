#pragma once
#include "../shared.h"
extern "C" {
	u void halt() {
		for (;;) {
			__asm__(
				"cli\r\n"
				"hlt"
			);
		}
	}

	u void softHalt() {
		__asm__("hlt");
	}
}
