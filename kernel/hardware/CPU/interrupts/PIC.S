# 00010001 binary. Enables initialization mode and we are sending ICW 4
#define ICW_1 $0x11

#Primary PIC control register
#define PIC_1_CTRL $0x20
# Secondary PIC control register
#define PIC_2_CTRL $0xA0

# Primary PIC data register
#define PIC_1_DATA $0x21
# Secondary PIC data register
#define PIC_2_DATA $0xA1

# IRQs 0-7 mapped to use interrupts 0x20-0x27
#define IRQ_0 $0x20
# IRQs 8-15 mapped to use interrupts 0x28-0x36
#define IRQ_8 $0x28
.global PICInit
.type PICInit, @function
PICInit:
	# Send ICW 1 - Begin initialization -------------------------
	# Setup to initialize the primary PIC. Send ICW 1
	movb ICW_1, %al
	outb %al, PIC_1_CTRL

	# Send ICW 2 - Map IRQ base interrupt numbers ---------------
	# Remember that we have 2 PICs. Because we are cascading with this second PIC, send ICW 1 to second PIC command register
	outb %al, PIC_2_CTRL

	# send ICW 2 to primary PIC
	movb IRQ_0, %al
	outb %al, PIC_1_DATA

	# send ICW 2 to secondary controller
	movb IRQ_8, %al
	outb %al, PIC_2_DATA

	# Send ICW 3 - Set the IR line to connect both PICs ---------
	# Send ICW 3 to primary PIC
	movb 0x4,%al				# 0x04 => 0100, second bit (IR line 2)
	outb %al, PIC_1_DATA		# write to data register of primary PIC

	# Send ICW 3 to secondary PIC
	movb $0x2,%al				# 010=> IR line 2
	outb %al, PIC_2_DATA		# write to data register of secondary PIC

	# Send ICW 4 - Set x86 mode --------------------------------
	movb $1,%al					# bit 0 enables 80x86 mode

	# send ICW 4 to both primary and secondary PICs
	outb %al, PIC_1_DATA
	outb %al, PIC_2_DATA

	# All done. Null out the data registers
	xor %al, %al
	outb %al, PIC_1_DATA
	outb %al, PIC_2_DATA
	ret