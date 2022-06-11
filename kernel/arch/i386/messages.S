.global _bootmsg
.type _bootmsg, @function
__bootmsgp1_ascii:
	.asciz "Techflash OS v"
__bootmsgp2_ascii:
	.asciz " Booting"
_bootmsg:
	call terminalInit
	push $__bootmsgp1_ascii
	call terminalWriteString
	mov $0xB8000, %edx
	movb $0x30 + CONFIG_KERN_VERSION_MAJOR, 28(%edx)
	movb $0x03, 29(%edx) # cyan
	movb $0x2E, 30(%edx) # .
	movb $0x03, 31(%edx) # cyan
	movb $0x30 + CONFIG_KERN_VERSION_MINOR, 32(%edx)
	movb $0x03, 33(%edx) # cyan
	movb $0x2E, 34(%edx) # .
	movb $0x03, 35(%edx) # cyan
	movb $0x30 + CONFIG_KERN_VERSION_PATCH, 36(%edx)
	movb $0x03, 37(%edx) # cyan
	push $0
	push $19
	call terminalMove
	push $__bootmsgp2_ascii
	call terminalWriteString
	movb $0xFE, 54(%edx) # .
	movb $0x04, 55(%edx) # red
