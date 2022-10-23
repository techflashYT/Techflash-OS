#include <kernel/fs/tar.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <kernel/environment.h>
#include <kernel/hardware/PIT.h> 

tarHeader_t *headers[2048];
uint64_t numHeaders = 0;
uint64_t getSize(const char* archive) {
	uint64_t size = 0;
	uint64_t count = 1;
	for (uint64_t i = 11; i > 0; i--, count *= 8) {
		size += ((archive[i - 1] - '0') * count);
	}

	return size;
}

uint64_t parseTar(void *address) {
	uint64_t i;
	for (i = 0; ;i++) {
		tarHeader_t *header = (tarHeader_t *)address;

		if (header->filename[0] == '\0') {
			break;
		}
		uint64_t size = getSize(header->size);

		headers[i] = header;

		address += ((size / 512) + 1) * 512;

		if (size % 512) {
			address += 512;
		}
	}
	numHeaders = i;
	return i;
}
uint32_t oct2bin(unsigned char *str, uint32_t size) {
	uint32_t i = 0;
	unsigned char *ptr = str;
	while (size-- > 0) {
		i *= 8;
		i += *ptr - '0';
		ptr++;
	}
	return i;
}

size_t readFile(unsigned char *archive, char *filename, char **out) {
	unsigned char *ptr = archive;
 
	while (!memcmp(ptr + 257, "ustar", 5)) {
		int filesize = oct2bin(ptr + 0x7c, 11);
		if (!memcmp(ptr, filename, strlen(filename) + 1)) {
			*out = ptr + 512;
			return filesize;
		}
		ptr += (((filesize + 511) / 512) + 1) * 512;
	}
	return 0;
}
void ls(const char* dir) {
	printf("dir: %s\r\n", dir);
	for (uint64_t i = 0; i < numHeaders; i++) {
		if (strstr(headers[i]->filename, dir) == NULL) {
			continue;
		}
		printf("%s\r\n", headers[i]->filename);
	}
}