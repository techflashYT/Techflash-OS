#include <stdbool.h>
/* imported virtual addresses, see linker script */
#include <external/bootboot.h>
extern BOOTBOOT bootboot;               // see bootboot.h
extern unsigned char environment[4096]; // configuration, UTF-8 text key=value pairs
extern uint8_t fb;                      // linear framebuffer mapped
typedef struct {
	
} environment_t;
extern environment_t env[CONFIG_KERN_MAXARGS];
environment_t handleEnv();