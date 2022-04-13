#include "../util.hpp"
#include <stdarg.h>
uint8_t fputs(const char *str, void *stream);
void putchar(char c, const uint8_t &x = 255, const uint8_t &y = 255);
uint8_t printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	uint8_t i = 0;
	const char *ptr;
	for (ptr = format; *ptr != '\0'; ptr++) {
		if (*ptr == '%') {
			ptr++;
			switch (*ptr) {
				case 's':
					fputs(va_arg(args, const char*), stdout);
					break;
				case '%':
					putchar('%');
					fputs(va_arg(args, const char*), stdout);
					i++;
					break;
			}
		}
	}
	va_end(args);
	return i;
}
