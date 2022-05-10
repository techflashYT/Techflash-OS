#include <stdio.h>
void assert(const char *message, const char *file, int line) {
	printf("ASSERT: Message: %s, File: %s, Line: %d\r\n", message, file, line);
	// TODO: Fix message not working right
}
