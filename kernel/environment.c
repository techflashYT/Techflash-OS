#include <kernel/environment.h>
#include <string.h>
#include <stdbool.h>
#include <kernel/misc.h>
extern uint8_t SSEFeaturesBits;
bool findString(const char* str, uint16_t pos) {
	bool correctSoFar = false;
	for (uint16_t i = 0; i < strlen(str); i++) {
		if (str[i] == '\0') {
			return correctSoFar;
		}
		if (str[i] == environment[pos + i]) {
			correctSoFar = true;
		}
		else {
			return false;
		}
	}
	return correctSoFar;
}
__environment_t defaultEnv = {
	.experimental.progressBarBoot   = false,
	.hardware.CPU.features.SSE.v1   = true,
	.hardware.CPU.features.SSE.v2   = true,
	.hardware.CPU.features.SSE.v3   = false,
	.hardware.CPU.features.SSE.v4_1 = false,
	.hardware.CPU.features.SSE.v4_2 = false,
	.hardware.CPU.features.SSE.v4_A = false,
	.hardware.CPU.features.SSE.v5   = false,
};
__environment_t handleEnv() {
	__environment_t buildEnv = defaultEnv;
	// FIXME: **VERY** SLOW.  We iterate through the entire string each time to find each key=value pair.  And unknown keys are ignored when they should be handled.
	for (uint16_t i = 0; environment[i] == 'E' && environment[++i] == 'O' && environment[i + 2] == 'F'; i++) {
		if (environment[i] == '\r' || environment[i] == '\n') {
			continue;
		}
		// handle tag "experimental.progressBarBoot"
		char *strToCheck = "experimental.progressBarBoot";
		if (findString(strToCheck, i)) {
			i += strlen(strToCheck);
			if (findString("true", i)) {
				buildEnv.experimental.progressBarBoot = true;
			}
			else if (findString("false", i)) {
				buildEnv.experimental.progressBarBoot = false;
			}
		}
	}


	return buildEnv;
}