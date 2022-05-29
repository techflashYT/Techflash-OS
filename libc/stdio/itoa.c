#include <stddef.h>
#include <stdint.h>
char* itoa(int res) {
	int testnum = res;
	int count = 0;
	if(res == 0) {
		count = 1;
	}
	else {
		while (testnum != 0) {
			testnum = testnum / 10;
			++count;
		}
	}
	static char ret[4];
	for (uint8_t i = 0; i < 4; i++) {
		ret[i] = '\0';
	}

	for (uint8_t i = 0; i < count; i++) {
		ret[count - i] = (char)((res % 10) + '0');
		res /= 10;	
	}

	return ret;
}
