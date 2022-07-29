.global IDTFlush
.type IDTFlush, @function
IDTFlush:
	lidt (%rdi)
	ret