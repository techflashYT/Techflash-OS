#include <stdbool.h>
/* imported virtual addresses, see linker script */
#include <external/bootboot.h>
extern BOOTBOOT bootboot;               // see bootboot.h
extern unsigned char environment[4096]; // configuration, UTF-8 text key=value pairs
extern uint8_t fb;                      // linear framebuffer mapped
typedef struct {
	const char key[50];
	const char value[50];
} __attribute__((packed)) environment_t; // we need this to be packed because it would take up way to much memory otherwise.  we're allocating 250 of them!
extern environment_t env[CONFIG_KERN_MAXARGS];
void handleEnv();