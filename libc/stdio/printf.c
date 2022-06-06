#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/sourceFileInfo.h>


char* itoa(int res);


__attribute__ ((unused)) static sourceFileInfo fileInfo = {
	.fileName = "libc/stdio/printf.c",
	.lastEditor = "Techflash",
	.lastEditDate = "May 16th, 2022",
	.lastEditReason = "Remove <kernel/assert.h> as it no long exists",
	.versionMajor = 0,
	.versionMinor = 0,
	.versionPatch = 2
};

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*)data;
	for (size_t i = 0; i < length; i++) {
		if (putchar(bytes[i]) == EOF) {
			return false;
		}
	}
	return true;
}

int printf(const char* restrict format, ...) {
	if (*format == '\0') {
		return 0;
	}
	va_list parameters;
	va_start(parameters, format);

	size_t written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
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
				return -1;
			}
			if (!print(format, amount)) {
				return -1;
			}
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c))) {
				return -1;
			}
			written++;
		}
		else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				print("string here", sizeof("string here"));
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len)) {
				print("string here", sizeof("string here"));
				return -1;
			}
			written += len;
		}
		else if (*format == 'd') {
			const char* numStr = itoa(va_arg(parameters, int));
			print(numStr, sizeof(numStr));
			format++;
		}
		else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len)) {
				return -1;
			}
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	if (written >= INT_MAX) {
		return INT_MAX;
	}
	else {
		return (int)written;
	}
}
