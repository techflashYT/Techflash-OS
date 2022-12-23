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
environment_t defaultEnv = {
	.experimental.progressBarBoot   = false,
	.hardware.CPU.features.SSE.v1   = true,
	.hardware.CPU.features.SSE.v2   = true,
	.hardware.CPU.features.SSE.v3   = false,
	.hardware.CPU.features.SSE.v4_1 = false,
	.hardware.CPU.features.SSE.v4_2 = false,
	.hardware.CPU.features.SSE.v4_A = false,
	.hardware.CPU.features.SSE.v5   = false,
};
environment_t handleEnv() {
	environment_t buildEnv = defaultEnv;
	// FIXME: **VERY** SLOW. Iterates through env each time to find a str. Unknown keys are ignored (bad).
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