#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <kernel/custom.h>
#include <kernel/hardware/serial.h>
/* NOTE: format specifiers are:
%c		character
%d, %i	signed int
%e		Scientific notation with lowercase e
%E		Scientific notation with uppercase E
%f		Float
%g		Shorter version of %e or %f
%G		Shorter version of %E or %f
%o		Signed octal
%s		String
%u		Unsigned int
%x		Unsigned Hex lowercase (0x0123456789abcdef)
%X		Unsigned Hex uppercase (0x0123456789ABCDEF)
%p		Pointer
%n		Nothing
%%		A literal %
*/

/*!
 * \brief Print a formatted string to the TTY.
 * \param format the format string.
 * \param ... the arguments to the format string.
 * \return the number of characters printed.
*/
int printf(const char* format, ...) {
	int ret = 0;
	va_list args;
	va_start(args, format);
	while (*format != '\0') {
		size_t maxrem = __INT_MAX__ - ret;
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%') {
				format++;
			}
			size_t amount = 1;
			while (format[amount] && format[amount] != '%') {
				amount++;
			}
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				va_end(args);
				return -1;
			}
			if (!putsNoTerminator(format, amount)) {
				return -1;
			}
			format += amount;
			ret += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(args, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (putchar(c)) {
				return -1;
			}
			ret++;
		}
		else if (*format == 's') {
			format++;
			const char* str = va_arg(args, const char*);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!puts(str)) {
				return -1;
			}
			ret += strlen(str);
		}
		else if (*format == 'd') {
			const char* numStr = itoa(va_arg(args, int), NULL, 10);
			putsNoTerminator(numStr, sizeof(numStr));
			format++;
		}
		else {
			format = format_begun_at;
			size_t len = strlen(format);
			serial.writeString(SERIAL_PORT_COM1, "printf: len: ");
			serial.writeString(SERIAL_PORT_COM1, itoa(len, NULL, 10));
			serial.writeString(SERIAL_PORT_COM1,"\r\n");
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!putsNoTerminator(format, len)) {
				return -1;
			}
			ret += len;
			format += len;
		}
	}
	va_end(args);
	return ret;
}