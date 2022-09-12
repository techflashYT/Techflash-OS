.global IDTFlush
.type IDTFlush, @function

IDTDesc:
	.word 4095
	.quad idtEntries
IDTFlush:
	lidt (IDTDesc)
	ret