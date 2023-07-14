#define FUNC_CHOOSER(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, ...) _f16
#define FUNC_RECOMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
#define CHOOSE_FROM_ARG_COUNT(F, ...) FUNC_RECOMPOSER((__VA_ARGS__, \
            F##_16, F##_15, F##_14, F##_13, F##_12, F##_11, F##_10, F##_9, F##_8,\
            F##_7, F##_6, F##_5, F##_4, F##_3, F##_2, F##_1, \
		))
#define NO_ARG_EXPANDER(FUNC) ,,,,,,,,,,,,,,,,FUNC ## _0
#define MACRO_CHOOSER(FUNC, ...) CHOOSE_FROM_ARG_COUNT(FUNC, NO_ARG_EXPANDER __VA_ARGS__ (FUNC))
#define MULTI_MACRO(FUNC, ...) MACRO_CHOOSER(FUNC, __VA_ARGS__)(__VA_ARGS__)

#define getCaller(...) MULTI_MACRO(__GETCALLER, __VA_ARGS__)
#define __GETCALLER_0()  __GETCALLER_1(0)
#define __GETCALLER_1(x) __builtin_extract_return_addr(__builtin_return_address(x))

#define MODULE(x) static const char *MODNAME = (x)
#define errorColor   0x00FF4444
#define warningColor 0x00FFD866

#ifdef MEM_DEBUG
#define malloc(x) mallocDebugWrapper((x), __FILE__, __LINE__)
#define realloc(x, y) reallocDebugWrapper((x), (y) __FILE__, __LINE__)
#define free(x) freeDebugWrapper((x), __FILE__, __LINE__)
#endif

#define DUMPREGS \
extern registers_t regsDump;\
asm("movq %%rax, %0\r\n"      : "=r" (regsDump.rax)     : );\
asm("movq %%rbx, %0\r\n"      : "=r" (regsDump.rbx)     : );\
asm("movq %%rcx, %0\r\n"      : "=r" (regsDump.rcx)     : );\
asm("movq %%rdx, %0\r\n"      : "=r" (regsDump.rdx)     : );\
asm("movq %%rdi, %0\r\n"      : "=r" (regsDump.rdi)     : );\
asm("movq %%rsi, %0\r\n"      : "=r" (regsDump.rsi)     : );\
asm("movq %%r8,  %0\r\n"      : "=r" (regsDump.r8)      : );\
asm("movq %%r9,  %0\r\n"      : "=r" (regsDump.r9)      : );\
asm("movq %%r10, %0\r\n"      : "=r" (regsDump.r10)     : );\
asm("movq %%r11, %0\r\n"      : "=r" (regsDump.r11)     : );\
asm("movq %%r12, %0\r\n"      : "=r" (regsDump.r12)     : );\
asm("movq %%r13, %0\r\n"      : "=r" (regsDump.r13)     : );\
asm("movq %%r14, %0\r\n"      : "=r" (regsDump.r14)     : );\
asm("movq %%r15, %0\r\n"      : "=r" (regsDump.r15)     : );\
asm("movq %%rsp, %0\r\n"      : "=r" (regsDump.userRsp) : );\
asm("movq %%rbp, %0\r\n"      : "=r" (regsDump.rbp)     : );\
asm("leaq 0x0(%%rip), %0\r\n" : "=r" (regsDump.rip)     : );\
registers_t *regs = &regsDump;
