#include <kernel/environment.h>
#include <kernel/fs/tar.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
tarHeader_t *headers[32];
char str1[128] = { '\0' };
char str2[128] = { '\0' };
typedef struct {
	char a[128];
	char b[128];
} TwoStrings;

TwoStrings strSplitRet;
unsigned int parseTar() {
	uint64_t initPointer = bootboot.initrd_ptr;
	unsigned int i;
 
	for (i = 0; ; i++) {
		tarHeader_t *header = (tarHeader_t *)bootboot.initrd_ptr;
		if (strcmp(&header->filename[0], "") != 0) {
			break;
		}

		headers[i] = header;
		bootboot.initrd_ptr += ((bootboot.initrd_size / 512) + 1) * 512;
		
		if (bootboot.initrd_size % 512) {
			bootboot.initrd_ptr += 512;
		}
	}
	bootboot.initrd_ptr = initPointer;
	return i;
}


char *readFile(char* fileToReadName) {
	tarHeader_t *header = (tarHeader_t *)bootboot.initrd_ptr;

	for (int i = 0; strcmp(headers[i]->filename, "") != 0; i++) {
		if (strcmp(fileToReadName, headers[i]->filename) == 0) {
			header = headers[i];
			break;
		}
	}

	char *contents = &((char *) header)[512]; // pointer shenanigans (:

	return contents;
}
// TODO: FIX MORE HERE
TwoStrings fullName;
TwoStrings nameInPath;
TwoStrings oldFullName;
TwoStrings oldNameInPath;

TwoStrings strsplit(char *str, char split) {
	// TODO: after malloc is fixed: char *p1 = malloc(128);
	// TODO: after malloc is fixed: char *p2 = malloc(128);
	char *p1 = str1;
	char *p2 = str2;
	size_t i = 0;
	int j = 0;
	
	for (; i < strlen(str) && str[i] != split; i++) {
		p1[i] = str[i];
	}
	i++;
	p1[i - 1] = '\0';
	for (; i < strlen(str); j++) {
		p2[j] = str[i];
		i++;
	}
	j++;
	p2[j - 1] = '\0';

	for (int i = 0; i < 128; i++) {
		strSplitRet.a[i] = p1[i];
		strSplitRet.b[i] = p2[i];
	}
	free(p1);
	free(p2);
	return strSplitRet;
}

void ls() {
	for (uint32_t i = 1; strcmp(headers[i]->filename, "") != 0; i++) {  
		memset(fullName.a, 0, 128);
		memset(fullName.b, 0, 128);
		memset(nameInPath.a, 0, 128);
		memset(nameInPath.b, 0, 128);
		memset(oldFullName.a, 0, 128);
		memset(oldFullName.b, 0, 128);
		memset(oldNameInPath.a, 0, 128);

		fullName   = strsplit(headers[i]->filename, '/');
		nameInPath = strsplit(fullName.b, '/');
		
		if (i > 1) {
			oldFullName   = strsplit(headers[i-1]->filename, '/');
			oldNameInPath = strsplit(oldFullName.b, '/');    
			if (strcmp(oldNameInPath.a, nameInPath.a) != 0) {
				printf("%s\r\n", nameInPath.a);
			}
		}
		else {
			printf("%s\r\n", nameInPath.a);
		}
	}
}