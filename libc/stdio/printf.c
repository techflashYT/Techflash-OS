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
// FIXME: Very busted, breaks the first character in mysterious ways and it seems to just stop printing after the first format specifier.
int printf(const char* format, ...) {
	int ret = 0;
	va_list args;
	va_start(args, format);
	while (*format != '\0') {
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%') {
				format++;
			}
			size_t amount = 1;
			while (format[amount] && format[amount] != '%') {
				amount++;
			}
			putsNoTerminator(format, amount);
			format += amount;
			ret += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(args, int /* char promotes to int */);
			putchar(c);
			ret++;
		}
		else if (*format == 's') {
			format++;
			const char* str = va_arg(args, const char*);
			puts(str);
			ret += strlen(str);
		}
		else if (*format == 'd' || *format == 'i') {
			const char* numStr = itoa(va_arg(args, int), NULL, 10);
			putsNoTerminator(numStr, sizeof(numStr));
			format++;
		}
		else if (*format == 'x') {
			const char* numStr = itoa(va_arg(args, int), NULL, 16);
			putsNoTerminator(numStr, sizeof(numStr));
			format++;
		}
		
		else {
			format = format_begun_at;
			size_t len = strlen(format);
			putsNoTerminator(format, len);
			ret += len;
			format += len;
		}
	}
	va_end(args);
	return ret;
}