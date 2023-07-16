#ifdef NDEBUG
	#define assert(expr) ((void)0)
#else
	#define assert(expr)\
		if (!(expr)) {\
			DUMPREGS;\
			char __assertStr[100];\
			sprintf(__assertStr, "Assertion `%s' failed in file " __FILE__ ":%d\r\n", #expr, __LINE__);\
			panic(__assertStr, regs);\
		}
#endif