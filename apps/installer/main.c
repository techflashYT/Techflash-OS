#include <stdio.h>
// #include <TFOS.h>
#include <installer.h>
int main(int argc, char* argv[])  {
	printf("Techflash OS Installer, v%d.%d.%d", INSTALLER_VERSION_MAJOR, INSTALLER_VERSION_MINOR, INSTALLER_VERSION_PATCH);

	puts("Welcome!  This installer will help you to install Techflash OS "); // puts is faster than printf for big blocks of text that don't have format specifiers.
	printf("v%d.%d.%d!", CONFIG_KERN_VERSION_MAJOR, CONFIG_KERN_VERSION_MINOR, CONFIG_KERN_VERSION_PATCH);

	/*

	FILE *readmeFileHandle = fopen("/etc/installerData/readme", "r");
	char readmeBuffer[CONFIG_README_BYTES];
	fgets(readmeBuffer, CONFIG_README_BYTES, readmeFileHandle);

	
	*/
	return 0;
}