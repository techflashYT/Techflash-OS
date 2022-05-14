#include <stddef.h>
char* itoa(int res) {
    int size = 0;
	if (res > 100) {
		size = 3;
	}
	else if (res > 10) {
		size = 2;
	}
	else {
		size = 1;
	}

    int t = res;
    static char ret[64] = {'\0'};
    ret[size] = '\0';
    t = res;
    int i = size - 1;
    while(i >= 0) {
        ret[i] = (char)(t % 10) + '0';
        t = t/10;
        i--;
    }

    return ret;
}
