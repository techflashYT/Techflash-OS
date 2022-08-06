#pragma once
#include <stdint.h>
typedef struct {
	uint64_t ds;
	uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
	uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t intNo, errCode;
	uint64_t rip, cs, rflags, userRsp, ss;
} registers_t;