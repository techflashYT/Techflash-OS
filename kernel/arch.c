#include <stdio.h>
#if defined(__x86_64__) || defined(_M_X64)
	char *ARCH_NameStr = "x86_64";
	extern void x86_64_Init();
#elif defined(i386) || defined(__i386__)
	char *ARCH_NameStr = "x86";
	extern void i386_Init();
#elif defined(__aarch64__) || defined(_M_ARM64)
	char *ARCH_NameStr = "AArch64";
	extern void AArch64_Init();
#else
	char *ARCH_NameStr = "???????";
#endif

void ARCH_Init() {
	#ifdef __x86_64__
		x86_64_Init();
	#else
		printf("[%s]: Unkown Arch: %s\r\n", __FUNC__, ARCH_NameStr);
	#endif
}