/*
	Original version Copyright 2017 - 2021 bzt (bztsrc@gitlab)
	as part of the 'bootboot' repository on GitLab.
	Original file availible at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/link.ld
*/
mmio        = 0xfffffffff8000000;              /* these are configurable for level 2 loaders */
fb          = 0xfffffffffc000000;
bootboot    = 0xffffffffffe00000;
environment = 0xffffffffffe01000;
initstack   = 65536;
PHDRS {
	boot PT_LOAD;                                /* one single loadable segment */

}
SECTIONS {
	. = 0xffffffffffe02000;
	.text : {
		KEEP(*(.text.boot)) *(.text .text.*)   /* code */
		*(.rodata .rodata.*)                   /* data */
		*(.data .data.*)
	} :boot
	.idt BLOCK(0x1000) : ALIGN(0x1000) {
		idtEntries = .;
		. = . + 0x1000;
	}
	.bss (NOLOAD) : {                          /* bss */
		. = ALIGN(16);
		*(.bss .bss.*)
		*(COMMON)
	} :boot

	/DISCARD/ : { *(.eh_frame) *(.comment) }
}

