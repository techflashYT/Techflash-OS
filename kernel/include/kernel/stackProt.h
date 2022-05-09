#include <stdint.h>
extern uintptr_t __stack_chk_guard;
__attribute__ ((noreturn)) void __stack_chk_fail(void);
