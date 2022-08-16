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
	char itoaBuf[32] = {'\0'};
	while (*format != '\0') {
		if (*format == '%') {
			format++;
			switch (*format) {
				case 'c':
					putchar(va_arg(args, int));
					format++;
					ret++;
					break;
				case 'd':
				case 'i':
					ret += puts(itoa(va_arg(args, int), itoaBuf, 10));
					format++;
					break;
				case 's':
					ret += puts(va_arg(args, char*));
					format++;
					break;
				case '%':
					putchar('%');
					format++;
					ret++;
					break;
				case 'x':
					ret += puts(itoa(va_arg(args, int), itoaBuf, 16));
					format++;
					break;
				default:
					putchar('%');
					putchar(*format);
					format++;
					break;
			}
		}
		putchar(*format);
		format++;
		ret++;
	}
	va_end(args);
	return ret;
}