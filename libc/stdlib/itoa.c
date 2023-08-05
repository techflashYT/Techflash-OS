#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
/*
	Based on "C++ version 0.4 char* style itoa"
	by Lukás Chmela.
	http://www.strudel.org.uk/itoa/
*/
char *itoa(int_fast64_t value, char* result, uint_fast8_t base) {
	// FIXME: (stupid workaround for a bug that I can't figure out, find real fix) if 0 just put zero and return
	if (value == 0) {
		*result = '0';
		*(result + 1) = '\0';
		return result;
	}
	// check that the base is valid
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	long tmpVal = 0;
	while (value) {
		tmpVal = value;
		value /= base;
		*ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" [(long)35 + (tmpVal - value * base)];
	}

	// Apply negative sign
	if (tmpVal < (long)0 && base == 10) {
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
