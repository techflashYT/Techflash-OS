# A makefile for various random things.
SHELL=/bin/bash
defaultError:
	@$(error This Makefile is not for external usage!  It is a utility Makefile for the rest of the project!)

sysroot:
	@$(shell \
		cd .. &&\
		rm -rf sysroot &&\
		mkdir sysroot &&\
		cd sysroot &&\
		mkdir -p bin usr/include/TechflashOS usr/src/TechflashOS boot installContents &&\
		cp ../bin/tfos_kernel.elf boot/tfos.elf &&\
		cp -r ../{.vscode,include,kernel,misc,util,.config,.giti*,.gita*,bootboot*,*.sh,configure,grub,README.md} usr/src/TechflashOS\
	)
