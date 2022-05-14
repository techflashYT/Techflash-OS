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
	char* ret;
	ret[size] = NULL;

	for (uint8_t i = 0; i < size; i++) {
		ret[size - i] = (res % 10) + '0';
		res /= 10;	
	}

    return ret;
}
