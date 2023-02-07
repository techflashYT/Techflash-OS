#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <kernel/panic.h>
#include <kernel/tty/tty.h>
/*
	Based on "C++ version 0.4 char* style itoa"
	by Lukás Chmela.
	http://www.strudel.org.uk/itoa/
*/
char* itoa(long value, char* result, int base) {
	// FIXME: (stupid workaround for a bug that I can't figure out, find real fix) if 0 just put zero and return
	if (value == 0) {
		*result = '0';
		*(result + 1) = '\0';
		return result;
	}
	// check that the base if valid
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	long tmp_value;
	while (value) {
		tmp_value = value;
		value /= base;
		*ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" [(long)35 + (tmp_value - value * base)];
	}

	// Apply negative sign
	if (tmp_value < (long)0 && base == 10) {
		*ptr++ = '-';
	}
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--   = *ptr1;
		*ptr1++  = tmp_char;
	}
	return result;
}
