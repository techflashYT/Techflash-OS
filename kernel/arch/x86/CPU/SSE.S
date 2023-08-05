/*.global x86_SSE_Init
.type x86_SSE_Init,@function
.section .rodata
.noSSEstr: .asciz "No SSE Support!!  Hanging...\r\n"
.noSSEstrEnd:

.section .data
mxcsr:
.long 0

.section .text
x86_SSE_Init:
	mov $0x1, %eax
	cpuid
	test $1 << 25, %edx
	jz .noSSE
	# SSE is available

	mov %cr4, %rax
	or $0x600, %ax		# set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
	mov %rax, %cr4

	fninit
	mov %cr0, %rax
	and $0xFFFB, %ax	# clear coprocessor emulation CR0.EM
	or $0x2, %ax		# set coprocessor monitoring  CR0.MP	
	mov %rax, %cr0

	// stmxcsr mxcsr
	movl $0x1F80, mxcsr
	ldmxcsr mxcsr


	mov $0x00, %rax
	ret


.noSSE:
	mov (SERIAL_PORT_COM1), %dx
	#ifdef __x86_64__
		leaq .noSSEstr(%rip), %rsi
	#else
		leaq .noSSEstr(%eip), %esi
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
	lea .loop(%rip), %rbx
	jmp *%rbx

.halt:
	cli
	hlt
	lea .halt(%rip), %rbx
	jmp *%rbx

*/