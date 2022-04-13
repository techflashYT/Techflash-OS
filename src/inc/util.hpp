#pragma once
#include "io.hpp"
#include "../shared.hpp"
#include <stdlib.h>
#include <stdint.h>

u static void halt() {
	for (;;) {
		asm(
			"cli\r\n"
			"hlt"
		);
	}
}

u static void softHalt() {
	asm("hlt");
}

