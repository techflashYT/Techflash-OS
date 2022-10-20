# A makefile for various random things.
SHELL=/bin/bash
defaultError:
	@$(error This Makefile is not for external usage!  It is a utility Makefile for the rest of the project!)

sysroot:
	@$(shell cd ..&&rm -rf sysroot&&mkdir sysroot&&cd sysroot&&mkdir -p bin usr/include/TechflashOS usr/src/TechflashOS boot installContents&&cp ../bin/tfos_kernel.elf boot/tfos.elf&&cp -r ../{.vscode,include,kernel,misc,util,.config,.git*,bootboot*,*.sh,configure,font.pf2,grub_bg.png,ISO_GRUB_CFG.cfg,README.md,testFile} usr/src/TechflashOS)
builddirs:
	@$(shell mkdir bin build lib&&cd build&&mkdir kernel libc&&cd kernel&&mkdir graphics hardware tty fs&&cd hardware&&mkdir serial PIT CPU IO&&cd CPU&&mkdir interrupts&&cd ../&&cd ../..&&cd libc&&mkdir stdio stdlib string custom&&cd ../..)	
