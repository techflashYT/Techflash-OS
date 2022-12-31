#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ioctl.h>
char *CONFIG_GRUB_OUTER_BG_COL;
char *CONFIG_GRUB_OUTER_FG_COL;
char *CONFIG_GRUB_MENU_BG_COL;
char *CONFIG_GRUB_MENU_FG_COL;
char *CONFIG_KERN_MAXARGS;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
// TODO: Make this less trash
#define CONFIGURE_VERSION  "0.2.0"
#define TFOS_VERSION_MAJOR 1
#define TFOS_VERSION_MINOR 0
#define TFOS_VERSION_PATCH 0
const char TFOS_VERSION[] = {TFOS_VERSION_MAJOR + '0', '.', TFOS_VERSION_MINOR + '0', '.', TFOS_VERSION_PATCH + '0', '\0'};

#define INVALIDCOLOR puts("\x1b[1;1H\x1b[2J\x1b[31mInvalid color name.\x1b[0m  Please use a valid color name.");\
					 return false;
static bool globalDone = false;
static bool kernelDone = false;
static bool libcDone   = false;
static bool grubDone   = false;
static bool endDone    = false;
static bool global(); static bool grub();
static bool kernel(); static bool libc();
static bool end();
static bool colorValid(const char *str, const char **validColors);
static void usage() {
	puts("./configure: Writes the config file for building Techflash OS.\r\n"
		 "ARGUMENTS:\r\n"
		 "    --no-max-screen: Forces the program to run, even if your screen is so small that it will look very ugly."
	);
	exit(1);
}
int main(int argc, char *argv[]) {
	printf("Techflash OS Configure Script v%s starting, built for Techflash OS v%s...\r\n", CONFIGURE_VERSION, TFOS_VERSION);
	struct winsize termSize;
	ioctl(0, TIOCGWINSZ, &termSize);
	if (argc > 2) {
		usage();
	}
	if (argc == 2) {
		if (strcmp(argv[1], "--no-max-screen") != 0) {
			usage();
		}
		puts("\x1b[1;33mWARNING\x1b[0m: This option is not supported!!!\r\n"
		     "Be warned.  Things will look weird."
		);
		goto afterScreenCheck;
	}
	if (termSize.ws_col < 100 ) {
		puts("Your terminal is too small.  Please resize it to at least 100 columns.\r\n"
		     "If you can't (like if you have basic linux console),\r\n"
			 "please run the script with --no-max-screen."
		);
		return 1;
	}
	if (termSize.ws_row < 10) {
		puts("Your terminal is too small.  Please resize it to at least 10 lines.");
		// don't comment about the linux console here since it will always be at least 25 lines.
		// the only way this could trigger is in a very small x term
		return 1;
	}
afterScreenCheck:
	// state handler loop
	while (true) {
		if      ((globalDone != true)) {if (global() == false) {continue;} }
		else if ((grubDone   != true)) {if (grub()   == false) {continue;} }
		else if ((kernelDone != true)) {if (kernel() == false) {continue;} }
		else if ((libcDone   != true)) {if (libc()   == false) {continue;} }
		else if ((endDone    != true)) {if (end()    == false) {continue;} }
		else                           {return 0;}
	}
	return 0;
}
static bool global() {
	puts("======================GLOBAL=====================");
	globalDone = true;
	return true;
}

static bool grub() {
	printf("======================GRUB=======================\r\n"
	       "CONFIG_GRUB_OUTER_BG_COL       Background color (GRUB color name) for around the boot menu, (default: black): "
	);
	CONFIG_GRUB_OUTER_BG_COL = malloc(30);
	memset(CONFIG_GRUB_OUTER_BG_COL, 0, 30);
	fgets(CONFIG_GRUB_OUTER_BG_COL, 30, stdin);
	if (CONFIG_GRUB_OUTER_BG_COL[0] == '\n') {
		strcpy(CONFIG_GRUB_OUTER_BG_COL, "black");
	}
	const char *validColors1[] = {
		"black", "blue", "green", "cyan",
		"red",   "magenta", "brown", "light-gray",
		0
	};
	if (!colorValid(CONFIG_GRUB_OUTER_BG_COL, validColors1)) {
		INVALIDCOLOR
	}

	printf("CONFIG_GRUB_OUTER_FG_COL       Foreground color (GRUB color name) for around the boot menu, (default: white): ");
	CONFIG_GRUB_OUTER_FG_COL = malloc(30);
	memset(CONFIG_GRUB_OUTER_FG_COL, 0, 30);
	fgets(CONFIG_GRUB_OUTER_FG_COL, 30, stdin);
	if (CONFIG_GRUB_OUTER_FG_COL[0] == '\n') {
		strcpy(CONFIG_GRUB_OUTER_FG_COL, "white");
	}
	const char *validColors2[] = {
		"black", "yellow", "green", "blue", "white", "red", "cyan", "magenta", "brown",
		"light-gray", "dark-gray", "light-blue", "light-green", "light-cyan", "light-red", "light-magenta",
		0
	};
	if (!colorValid(CONFIG_GRUB_OUTER_FG_COL, validColors2)) {
		INVALIDCOLOR
	}

	printf("CONFIG_GRUB_MENU_BG_COL        Background color (GRUB color name) for the boot menu, (default: light-gray): ");
	CONFIG_GRUB_MENU_BG_COL = malloc(30);
	memset(CONFIG_GRUB_MENU_BG_COL, 0, 30);
	fgets(CONFIG_GRUB_MENU_BG_COL, 30, stdin);
	if (CONFIG_GRUB_MENU_BG_COL[0] == '\n') {
		strcpy(CONFIG_GRUB_MENU_BG_COL, "light-gray");
	}
	if (!colorValid(CONFIG_GRUB_MENU_BG_COL, validColors1)) {
		INVALIDCOLOR
	}
	printf("CONFIG_GRUB_MENU_FG_COL        Foreground color (GRUB color name) for the boot menu, (default: white): ");
	CONFIG_GRUB_MENU_FG_COL = malloc(30);
	memset(CONFIG_GRUB_MENU_FG_COL, 0, 30);
	fgets(CONFIG_GRUB_MENU_FG_COL, 30, stdin);
	if (CONFIG_GRUB_MENU_FG_COL[0] == '\n') {
		strcpy(CONFIG_GRUB_MENU_FG_COL, "white");
	}
	if (!colorValid(CONFIG_GRUB_MENU_FG_COL, validColors2)) {
		INVALIDCOLOR
	}
	grubDone = true;
	return 0;
}

static bool kernel() {
	printf("=====================KERNEL======================\r\n"
	       "CONFIG_KERN_MAXARGS            The maximum number of arguments that the kernel will allocate space for, 0 - 65535 (default: 250): "
	);
	CONFIG_KERN_MAXARGS = malloc(30);
	memset(CONFIG_KERN_MAXARGS, 0, 30);
	fgets(CONFIG_KERN_MAXARGS, 30, stdin);
	if (CONFIG_KERN_MAXARGS[0] == '\n') {
		strcpy(CONFIG_KERN_MAXARGS, "250");
	}
	bool valid = true;
	for (uint8_t i = 0; i != strlen(CONFIG_KERN_MAXARGS); i++) {
		if (!isdigit(CONFIG_KERN_MAXARGS[i]) && CONFIG_KERN_MAXARGS[i] != '-') {
			valid = false;
		}
	}
	if (!valid) {
		puts("\x1b[1;1H\x1b[2J\x1b[31mNot a number\x1b[0m!  Please choose a valid number between 0 and 65535");
		return false;
	}
	int64_t temp = atoi(CONFIG_KERN_MAXARGS);
	if (temp < 0 || temp > 65535) {
		puts("\x1b[1;1H\x1b[2J\x1b[31mInvalid number\x1b[0m, please choose a value between 0 and 65535");
		return false;
	}
	kernelDone = true;
	return true;
}


static bool libc() {
	puts("===================LIBC/LIBK=====================");
	libcDone = true;
	return 0;
}
static bool edit() {
	puts("\x1b[0m\x1b[1;1H\x1b[2J"
		 "What would you like to edit?\r\n"
		 "\x1b[9m1. Global\x1b[0m\r\n"
		 "2. GRUB\r\n"
		 "\x1b[9m3. Kernel\x1b[0m\r\n"
		 "\x1b[9m4. libc/libk\x1b[0m\r\n"
		 "5. Review\r\n"
	);
	char *editChoice = malloc(30);
	fgets(editChoice, 30, stdin);
start:
	switch (editChoice[0]) {
		case '1': {
			puts("\x1b[1;1H\x1b[2J\x1b[33mSorry!  This part doesn't have any configuration options yet!\x1b[0m\r\n");
			goto start;
		}
		case '2': {
			grub();
			break;
		}
		case '3': {
			puts("\x1b[1;1H\x1b[2J\x1b[33mSorry!  This part doesn't have any configuration options yet!\x1b[0m\r\n");
			goto start;
		}
		case '4': {
			puts("\x1b[1;1H\x1b[2J\x1b[33mSorry!  This part doesn't have any configuration options yet!\x1b[0m\r\n");
			goto start;
		}
		case '5': {
			end();
			break;
		}
		default: {
			puts("\x1b[1;1H\x1b[2J\x1b[31mInvalid option!  Please try again!\x1b[0m");
			goto start;
		}
	}
	free(editChoice);
	return true;
}
static bool end() {
	char *HEADER = malloc(216);
	sprintf(HEADER,
		"# Config file generated by Techflash OS Configure Script v%s\r\n"
		"# Made for Techflash OS v%s\r\n"
		"# !!! DO NOT MANUALLY EDIT THIS FILE !!! USE THE CONFIGURE SCRIPT !!!\r\n"
		"###### START AUTOGENERATED CONFIG ######\r\n", CONFIGURE_VERSION, TFOS_VERSION
	);
	char *CONFIG = malloc(350);
	sprintf(CONFIG,
		"CONFIGURE_VERSION=%s\r\n"
		"CONFIG_KERN_VERSION_MAJOR=%d\r\n"
		"CONFIG_KERN_VERSION_MINOR=%d\r\n"
		"CONFIG_KERN_VERSION_PATCH=%d\r\n"
		"CONFIG_KERN_MAXARGS=%s\r\n"
		"CONFIG_GRUB_OUTER_BG_COL=%s\r\n"
		"CONFIG_GRUB_OUTER_FG_COL=%s\r\n"
		"CONFIG_GRUB_MENU_BG_COL=%s\r\n"
		"CONFIG_GRUB_MENU_FG_COL=%s\r\n",
		CONFIGURE_VERSION,
		TFOS_VERSION_MAJOR, TFOS_VERSION_MINOR, TFOS_VERSION_PATCH, CONFIG_KERN_MAXARGS,
		CONFIG_GRUB_OUTER_BG_COL, CONFIG_GRUB_OUTER_FG_COL, CONFIG_GRUB_MENU_BG_COL, CONFIG_GRUB_MENU_FG_COL
	);
	char *FOOTER = malloc(41);
	strcpy(FOOTER, "####### END AUTOGENERATED CONFIG #######");

	puts("\r\n\r\n=====================CONFIG======================");
	puts(CONFIG);
	printf("Does this configuration look good? (Y/n): ");
	char confirm = getchar();
	if (confirm == '\n') {
		confirm = 'y';
	}
	if (confirm == 'y' || confirm == 'Y') {
		char *cwd = getcwd(NULL, 0);
		char *buffer = malloc(strlen(cwd) + 9);
		strcpy(buffer, cwd);
		strcpy(buffer + strlen(buffer), "/.config");
		printf("Your configuration will be saved in %s\r\n", buffer);
		FILE *fileHandle = fopen(buffer, "w");
		fputs(HEADER, fileHandle);
		fputs(CONFIG, fileHandle);
		fputs(FOOTER, fileHandle);
		fclose(fileHandle);
	}
	else if (confirm == 'n' || confirm == 'N') {
		if (!edit()) {
			edit();
		}
		else {
			return false;
		}
	}
	else {
		puts("\x1b[1;1H\x1b[2J\x1b[31mInvalid input.\x1b[0m  Please enter y or n.");
		return false;
	}
	endDone = true;
	return true;
}
static bool colorValid(const char *str, const char **validColors) {
	bool valid = false;
	for (uint8_t i = 0; validColors[i] != 0; i++) {
		if (strcmp(str, validColors[i]) == 0) {
			valid = true;
		}
	}
	return valid;
}
#pragma GCC diagnostic pop