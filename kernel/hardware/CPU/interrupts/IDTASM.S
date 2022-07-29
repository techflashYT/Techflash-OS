.global IDTFlush
.type IDTFlush, @function
IDTFlush:
	mov 4(%esp), %eax
	lidt (%eax)
	ret