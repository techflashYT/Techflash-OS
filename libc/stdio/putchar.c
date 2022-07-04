#include <stdio.h>
#include <stdbool.h>
#include <kernel/tty.h>
// TODO: Serial output?
int putchar(int ic) {
#if defined(__is_libk)
	char c = (char)ic;
	if (c == '\0') {
		return 0;
	}
	terminalWrite(&c, sizeof(c));
#else
	_write();
#endif
	return ic;
	
}
