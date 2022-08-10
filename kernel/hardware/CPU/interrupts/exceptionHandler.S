.global exceptionHandler
.type exceptionHandler,@function
.global panic
.type panic,@function
.section .data
panicStr:
.asciz "exceptionHandler(): Unhandled exception in kernel code!"

.section .text
exceptionHandler:
	# TODO: Registers
	mov $panicStr, %rdi
	call panic
	# Infinite loop
	1:
	jmp 1b
