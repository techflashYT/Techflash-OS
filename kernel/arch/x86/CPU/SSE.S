.global x86_SSE_Init
.type x86_SSE_Init,@function
.section .rodata
.noSSEstr: .asciz "No SSE Support!!  Hanging...\r\n"

.section .text
x86_SSE_Init:
	mov $0x1, %eax
	cpuid
	test $1 << 25, %edx
	jz .noSSE
	# SSE is available
	mov $0x00, %rax
	ret


.noSSE:
	mov (SERIAL_PORT_COM1), %dx
	#ifdef __x86_64__
		leaq .noSSEstr(%rip), %rsi
	#else
		leaq .noSSEstr(%rip), %esi
	#endif

.loop:
	# NOTE: doing this manually here, since the serial write routines are in C, which would use SSE.
    lodsb

	# Is it NULL?
    testb %al, %al

	# yes, get out of here
    jz .halt

	# no, write the next char
    outb %al, %dx
	# loop back
	jmp .loop

.halt:
	cli
	hlt
	jmp .halt
