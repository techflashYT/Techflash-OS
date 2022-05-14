#include <stddef.h>
#include <stdint.h>
char* itoa(int res) {
    uint8_t size = 0;
	if (res > 100) {
		size = 3;
	}
	else if (res > 10) {
		size = 2;
	}
	else {
		size = 1;
	}
	static char ret[4];
	for (uint8_t i = 0; i < 4; i++) {
		ret[i] = '\0';
	}

	for (uint8_t i = 0; i < size; i++) {
		ret[size - i] = (char)((res % 10) + '0');
		res /= 10;	
	}

    return ret;
}
