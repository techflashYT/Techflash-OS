#include "../inc/util.hpp"
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
static void printDec(uint_fast32_t value, uint_fast32_t width, int_fast8_t *buf, int_fast32_t *ptr ) {
	uint_fast32_t n_width = 1;
	uint_fast32_t i = 9;
	while (value > i && i < UINT32_MAX) {
		n_width += 1;
		i *= 10;
		i += 9;
	}

	int printed = 0;
	while (n_width + printed < width) {
		buf[*ptr] = '0';
		*ptr += 1;
		printed += 1;
	}

	i = n_width;
	while (i > 0) {
		uint_fast32_t n = value / 10;
		int_fast32_t r = value % 10;
		buf[*ptr + i - 1] = r + '0';
		i--;
		value = n;
	}
	*ptr += n_width;
}
// static void printHex(uint_fast32_t value, uint_fast32_t width, int_fast8_t *buf, int_fast32_t *ptr) {
// 	int_fast32_t i = width;

// 	if (i == 0) {
// 		i = 8;   
// 	}

// 	uint_fast32_t n_width = 1;
// 	uint_fast32_t j = 0x0F;
// 	while (value > j && j < UINT32_MAX) {
// 		n_width += 1;
// 		j *= 0x10;
// 		j += 0x0F;
// 	}

// 	while (i > (int)n_width) {
// 		// buf[*ptr] = '0';
// 		*ptr += 1;
// 		i--;
// 	}

// 	i = (int_fast32_t)n_width;
// 	while (i-- > 0) {
// 		buf[*ptr] = "0123456789abcdef"[(value>>(i*4))&0xF];
// 		*ptr += + 1;
// 	}
// }




size_t vasprintf(int_fast8_t *buf, const char *fmt, va_list args) {
	uint_fast8_t *s;
	int_fast32_t ptr = 0;
	uint_fast32_t len = strlen(fmt);
	for (uint_fast16_t i = 0; i < len && fmt[i]; ++i) {
		if (fmt[i] != '%') {
			buf[ptr++] = fmt[i];
			continue;
		}
		++i;
		uint_fast16_t arg_width = 0;
		while (fmt[i] >= '0' && fmt[i] <= '9') {
			arg_width *= 10;
			arg_width += fmt[i] - '0';
			++i;
		}
		/* fmt[i] == '%' */
		switch (fmt[i]) {
			case 's': /* String pointer -> String */
				s = (uint_fast8_t *)va_arg(args, char *);
				while (*s) {
					buf[ptr++] = *s++;
				}
				break;
			case 'c': /* Single character */
				buf[ptr++] = (char)va_arg(args, int);
				break;
			// case 'x': /* Hexadecimal number */
			// 	printHex((uint_fast64_t)va_arg(args, uint_fast64_t), arg_width, buf, &ptr);
			// 	break;
			case 'd': /* Decimal number */
				printDec((uint_fast64_t)va_arg(args, uint_fast64_t), arg_width, buf, &ptr);
				break;
			case '%': /* Escape */
				buf[ptr++] = '%';
				break;
			default: /* Nothing at all, just dump it */
				buf[ptr++] = fmt[i];
				break;
		}
	}
	/* Ensure the buffer ends in a null */
	buf[ptr] = '\0';
	return ptr;
}

void placech(uint_fast8_t c, uint_fast8_t x, uint_fast8_t y, uint_fast8_t attr) {
	uint_fast16_t *where;
	uint_fast32_t att = attr << 8;
	where = vga_buffer + (y * VGA_WIDTH + x);
	*where = c | att;
}

int _notReady = 1;
int _off = 0;


/* 
	Print function, prints a string to the screen by writing into VGA RAM.

	ARGUMENTS:
		char* string, the string to print
		uint_fast8_t color, the color to print the string in, (0-15), default is 15 (white)
	
	RETURNS: Nothing.
*/
void kprintf(int_fast8_t x, uint_fast8_t y, uint_fast8_t color, const char* str, ...) {
	int_fast8_t buffer[512] {-1};
	va_list args;
	va_start(args, str);
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-variable"
	int_fast32_t out = vasprintf(buffer, str, args);
	#pragma GCC diagnostic pop
	/* We're done with our arguments */
	va_end(args);
	uint_fast8_t *c = (uint_fast8_t *)buffer;
	while (*c) {
		if (_off) {
			if (*c > 'a' && *c < 'z') {
				_off = 0;
			} else {
				c++;
				continue;
			}
		}
		else if (*c == '\033') {
			c++;
			if (*c == '[') {
				_off = 1;
			}
		}
		else if (*c == '\n') {
			y += 1;
			x = 0;
		}
		else {
			placech(*c, x, y, color);
			x++;
		}
		if (x == 80) {
			x = 0;
			y++;
		}
		if (y == 24) {
			y = 0;
			x = 0;
		}
		c++;
	}
}
#pragma GCC diagnostic pop
