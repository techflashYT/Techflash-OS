#include <stdint.h>
#include <stddef.h>
typedef struct {
	char filename[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char lastModified[12];
	char checksum[8];
	char typeFlag[1];
} tarHeader_t;

uint64_t getSize(const char* archive);
uint64_t parseTar(void *address);
void ls(const char* dir);
size_t readFile(unsigned char *archive, char *filename, unsigned char **out);