#include <stdbool.h>
#include <external/limine.h>
typedef struct {
	const char key[50];
	const char value[50];
} __attribute__((packed)) environment_t; // we need this to be packed because it would take up way too much memory otherwise.  we're allocating 250 of them!
extern environment_t env[CONFIG_KERN_MAXARGS];
extern void handleEnv();
extern volatile struct limine_framebuffer *fb;