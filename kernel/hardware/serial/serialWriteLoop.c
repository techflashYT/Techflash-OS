#include <stdint.h>
void __serialWriteString(const uint16_t port, const char* value) {
	while(*value != '\0') {
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
		__serialWrite(port, *value);
		#pragma GCC diagnostic pop
		value++;
	}
}