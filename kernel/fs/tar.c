#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <kernel/fs/tar.h>

tarHeader_t *headers[2048];
uint_fast64_t numHeaders = 0;
uint_fast64_t getSize(const char* archive) {
	uint_fast64_t size = 0;
	uint_fast64_t count = 1;
	for (uint_fast64_t i = 11; i > 0; i--, count *= 8) {
		size += ((archive[i - 1] - '0') * count);
	}

	return size;
}

uint_fast64_t parseTar(void *address) {
	uint_fast64_t i;
	for (i = 0; ;i++) {
		tarHeader_t *header = (tarHeader_t *)address;

		if (header->filename[0] == '\0') {
			break;
		}
		uint_fast64_t size = getSize(header->size);

		headers[i] = header;

		address += ((size / 512) + 1) * 512;

		if (size % 512) {
			address += 512;
		}
	}
	numHeaders = i;
	return i;
}
size_t oct2bin(uint8_t *str, size_t size) {
	size_t i = 0;
	uint8_t *ptr = str;
	while (size-- > 0) {
		i *= 8;
		i += *ptr - '0';
		ptr++;
	}
	return i;
}

size_t readFile(uint8_t *archive, char *filename, uint8_t **out) {
	size_t filesize = 0;
	while (!memcmp(archive + 257, "ustar", 5)) {
		filesize = oct2bin(archive + 0x7c, 11);
		if (!memcmp(archive, filename, strlen(filename) + 1)) {
			*out = archive + 512;
			return filesize;
		}
		archive += (((filesize + 511) / 512) + 1) * 512;
	}
	return 0;
}
void ls(const char* dir) {
	for (uint_fast64_t i = 0; i < numHeaders; i++) {
		if (strstr(headers[i]->filename, dir) == NULL) {
			continue;
		}
		puts(headers[i]->filename);
		puts("\r\n");
	}
}