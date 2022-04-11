#include "../util.hpp"

size_t strlen(const char *str) {
	int i = 0;
	while (str[i] != '\0') {
		++i;
	}
	return i;
}
