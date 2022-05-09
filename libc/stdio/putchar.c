#include <stdio.h>
#include <stdbool.h>
#if defined(__is_libk)
#include <kernel/tty.h>
#endif
bool checkIfNextCharIsNewline = false;
int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	if (c == '\r') {
		checkIfNextCharIsNewline = true;
		return ic;
	}
	if (c == '\n' && checkIfNextCharIsNewline) {
		terminal_write(&c, sizeof(c));
		checkIfNextCharIsNewline = false;
	}
	if (c != '\n' && checkIfNextCharIsNewline) {
		terminal_write(&c, sizeof(c));
	}
		
	terminal_write(&c, sizeof(c));
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}
