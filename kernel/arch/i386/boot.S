# Constants for multiboot 2 Header
.set MAGIC,    0xE85250D6    # magic number
.set ARCH,     0x00000000    # 32-bit x86
.set HEAD_LEN, 0x00000010    # Header length
.set CHECKSUM, -(HEAD_LEN + ARCH + MAGIC) # checksum, must be a uint32 that, when the above things are added to it, equals 0.  So all we do is do the exact same thing, but make it negative.  Like how -1 + 1 = 0, the same principle applies here.

# Multiboot 2 header
.section .multiboot
.long MAGIC
.long ARCH
.long HEAD_LEN
.long CHECKSUM
# ###### FLAGS FOR REAL 640x480 FRAMEBUFFER #####
# .long 0
# .short 5 # type = 5
# .short 0 # flags = 0
# .long 20 # size = 20
# .long 640 # width
# .long 480 # height
# .long 8 # depth
# .long 0 # END
# But rather than that, lets just use basic console because it's so much easier to use
.word 4
.word 0
.long 12
.long 0x03
.balign 8
# Request some info
.word 1
.word 0
.long 4*6+8
.long 5 # BIOS boot device
.long 1 # command line
.long 3 # modules
.long 9 # ELF symbols
.long 6 # memory map
.long 10 # APM table
.balign 8
.long 0
.long 0
.long 0
# Reserve a stack for the initial thread.
# .global CONFIG_KERN_VERSION_MAJOR
# .global CONFIG_KERN_VERSION_MINOR
# .global CONFIG_KERN_VERSION_PATCH
.global printf
.type printf, @function

.section .bss
.align 16
stackBottom:
.skip 131072 # 128K of stack
stackTop:
# The kernel entry point.
.section .text
.global _start
.global _disableCursor
.type _disableCursor, @function
.type _start, @function

_start:
	movl $stackTop, %esp
	pushl %ebx
	pushl %eax
	mov $0xB8000, %ebx
	movb $0x54, 0(%ebx) # T
	movb $0x65, 2(%ebx) # e
	movb $0x63, 4(%ebx) # c
	movb $0x68, 6(%ebx) # h
	movb $0x66, 8(%ebx) # f
	movb $0x6C, 10(%ebx) # l
	movb $0x61, 12(%ebx) # a
	movb $0x73, 14(%ebx) # s
	movb $0x68, 16(%ebx) # h
	movb $0x20, 18(%ebx) # space
	movb $0x4F, 20(%ebx) # O
	movb $0x53, 22(%ebx) # S
	movb $0x20, 24(%ebx) # space
	movb $0x76, 26(%ebx) # v
	movb $0x30 + CONFIG_KERN_VERSION_MAJOR, 28(%ebx)
	movb $0x03, 29(%ebx) # cyan
	movb $0x2E, 30(%ebx) # .
	movb $0x03, 31(%ebx) # cyan
	movb $0x30 + CONFIG_KERN_VERSION_MINOR, 32(%ebx)
	movb $0x03, 33(%ebx) # cyan
	movb $0x2E, 34(%ebx) # .
	movb $0x03, 35(%ebx) # cyan
	movb $0x30 + CONFIG_KERN_VERSION_PATCH, 36(%ebx)
	movb $0x03, 37(%ebx) # cyan
	movb $0x20, 38(%ebx) # space
	movb $0x42, 40(%ebx) # B
	movb $0x6F, 42(%ebx) # o
	movb $0x6F, 44(%ebx) # o
	movb $0x74, 46(%ebx) # t
	movb $0x69, 48(%ebx) # i
	movb $0x6E, 50(%ebx) # n
	movb $0x67, 52(%ebx) # g
	movb $0x2E, 54(%ebx) # .
	movb $0x04, 55(%ebx) # red

	# Call the global constructors.
	call _init
	movb $0x2E, 56(%ebx) # .
	movb $0x06, 57(%ebx) # brown (orange)
	# Initialize the floating point unit.
	finit
	movb $0x2E, 58(%ebx) # .
	movb $0x07, 59(%ebx) # yellow
	# TODO: Initialize the PIC and PIT
	
	
	# Check if A20 Gate is already enabled from grub 
	pushal
	mov $0x112345, %edi  # odd megabyte address.
	mov $0x012345, %esi  # even megabyte address.
	mov %esi, (%esi)     # making sure that both addresses contain diffrent values.
	mov %esi, (%esi)     # (if A20 line is cleared the two pointers would point to the address 0x012345 that would contain 0x112345 (edi)) 
	cmpsd             # compare addresses to see if the're equivalent.
	popal
	movb $0x2E, 60(%ebx) # .
	movb $0x0A, 61(%ebx) # lightgreen
	jne __bootstg2 # if A20 is on, jmp to call boot stage2.
	# Enable A20 Gate if it's not enabled.
	in $0x92, %al
	or $2, %al
	out %al, $0x92
	jmp __bootstg2

_disableCursor:
	pushf
	push %eax
	push %edx

	mov $0x3d4, %dx
	mov $0xa, %al
	out %al, %dx

	inc %dx
	mov $0x20, %al
	out %al, %dx

	pop %edx
	pop %eax
	popf
	ret
nofpu:
	mov $0x0, %eax
	ret
__bootstg2:
	movb $0x2E, 62(%ebx) # .
	movb $0x02, 63(%ebx) # green
	call _disableCursor
	# Just in case interrupts are enabled, disable them.
	cli
	# Also just in case, clear all the regs
	xor %eax, %eax
	xor %ebx, %ebx
	xor %esi, %esi
	xor %edi, %edi
	xor %ebp, %ebp
	xor %ecx, %ecx
	xor %edx, %edx
	# Transfer control to the main kernel.
	movb $0x2E, 64(%ebx) # .
	movb $0x03, 65(%ebx) # yellow
	call kernelMain

	# Hang if kernelMain unexpectedly returns.
	cli
1:	hlt
	jmp 1b
.size __bootstg2, . - _start
