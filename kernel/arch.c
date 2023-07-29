#include <stdio.h>
char *ARCH_NameStr =
#if defined(__x86_64__) || defined(_M_X64)
	"x86_64";
	extern void x86_64_Init();
#elif defined(i386) || defined(__i386__)
	"x86";
	extern void i386_Init();
#elif defined(__aarch64__) || defined(_M_ARM64)
	"ARM64";
	extern void AArch64_Init();
#else
	"Unknown";
#endif

void ARCH_Init() {
	#ifdef __x86_64__
		x86_64_Init();
	#else
		printf("[%s]: Unkown Arch: %s\r\n", __FUNC__, ARCH_NameStr);
	#endif
}