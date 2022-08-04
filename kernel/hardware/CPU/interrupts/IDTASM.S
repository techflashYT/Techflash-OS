.global IDTFlush
.type IDTFlush, @function

IDTDesc:
	.word 4095
	.quad idtEntries
IDTFlush:
	xchgw %bx, %bx
	lidt (%rdi)
	sti
	ret