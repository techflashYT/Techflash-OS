#pragma once
#include "../shared.hpp"
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
