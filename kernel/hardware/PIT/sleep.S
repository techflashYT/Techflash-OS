.global timerIRQ
.type timerIRQ, @function
.global sleep
.type sleep,@function

timerIRQ:
	xchgw %bx, %bx
	push %rax
	mov __sleepCountdown, %rax
	orq %rax, %rax
	jz __timerDone
	movq __sleepCountdown, %rax
	decq %rax
	movq %rax, __sleepCountdown
__timerDone:
	popq %rax
	iretq

sleep:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rax
	movq 8(%rbp),%rax  # rax has value of sole argument
	movq %rax,__sleepCountdown
__sleepLoop:
	cli # can't be interrupted for test
	movq __sleepCountdown,%rax
	orq %rax,%rax
	jz __sleepDone
	sti
	push %ax
	push %dx

	# Wait until the interrupt is done
	# hlt  FIXME: BROKEN
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	pop %dx
	pop %ax
	jmp __sleepLoop
__sleepDone:
	sti
	popq %rax
	popq %rbp
	ret
