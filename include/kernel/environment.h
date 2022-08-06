#include <stdbool.h>
/* imported virtual addresses, see linker script */
#include <external/bootboot.h>
extern BOOTBOOT bootboot;               // see bootboot.h
extern unsigned char environment[4096]; // configuration, UTF-8 text key=value pairs
extern uint8_t fb;                      // linear framebuffer mapped
typedef struct {
	struct {
		bool progressBarBoot;
	} experimental;
	struct {
		struct {
			struct {
				struct {
					bool v1;
					bool v2;
					bool v3;
					bool v4_1;
					bool v4_2;
					bool v4_A;
					bool v5;
				} SSE;
			} features;
		} CPU;
	} hardware;
} __environment_t;
extern __environment_t env;
__environment_t handleEnv();