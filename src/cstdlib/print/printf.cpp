#include "../util.hpp"
#include <stdarg.h>
void fputs(const char *str, void *stream);
void putchar(char c, uint8_t x = 255, uint8_t y = 255);
uint8_t printf(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	uint8_t i = 0;
	const char *ptr;
	for (ptr = format; *ptr != '\0'; ptr++) {
		if (*ptr == '%') {
			ptr++;
			switch (*ptr) {
				case 's':
					i += fputs(va_arg(ap, const char*), stdout);
					break;
				case '%':
					putchar('%');
					i++
					break;
				}
			}
		else {
			putchar(*ptr, 9, 0);
			i++;
		}
	}
	va_end(ap);
	return i;
}
