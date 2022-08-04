#include <kernel/misc.h>
#include <string.h>
// FIXME: Something here is BAD.  Somehow it appears to be corrupting the stack.
char *strcat(char* restrict destination, const char* restrict source) {
	// ptr to end of dest
	char *ptr = destination + strlen(destination);
	
	// append to dest
	while (*source != '\0') {
		*ptr++ = *source++;
	}

	// null terminate
	*ptr = '\0';

	// return dest
	return destination;
}