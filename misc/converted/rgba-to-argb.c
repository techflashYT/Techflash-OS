#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "needs filename\r\n");
		exit(1);
	}
	FILE *fp = fopen(argv[1], "r+b");

	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// perror("perror");
	// abort();

	uint8_t *addr = malloc(size);
	size_t ret = fread(addr, 1, size, fp);
	// printf("addr: %p\r\nsize: %lu\r\nfp: %p\r\nfile: %d\r\nerrno: %d\r\n", addr, size, fp, file, errno);
	printf("addr: %p\r\nsize: %lu\r\nfp: %p\r\nfread: %lu", addr, size, fp, ret);
	perror("perror");
	puts("");
	for (uint64_t i = 0; i < size; i += 4) {
		uint8_t r = addr[i + 0];
		uint8_t g = addr[i + 1];
		uint8_t b = addr[i + 2];
		uint8_t a = addr[i + 3];
		addr[i + 0] = a;
		addr[i + 1] = r;
		addr[i + 2] = g;
		addr[i + 3] = b;
	}

	ret = fwrite(addr, 1, size, fp);
	printf("fwrite: %lu\r\n", ret);
	perror("perror");
	if (errno != 0) {
		exit(1);
	}

	fclose(fp);
	free(addr);
	return 0;
}