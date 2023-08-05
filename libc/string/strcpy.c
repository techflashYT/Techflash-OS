#include <stdlib.h>
#include <string.h>
char* strcpy(char* destination, const char* source) {
	// return if no memory is allocated to the destination
	if (destination == NULL) {
		return NULL;
	}
 
	// take a pointer pointing to the beginning of the destination string
	char *ptr = destination;
 
	// copy the C-string pointed by source into the array
	// pointed by destination
	while (*source != '\0') {
		*destination = *source;
		destination++;
		source++;
	}
 
	// include the terminating null character
	*destination = '\0';
 
	// the destination is returned by standard `strcpy()`
	return ptr;
}
