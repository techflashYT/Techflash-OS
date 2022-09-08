# Based on kernel/Makefile: For copyright notices, please view that file.
WARN   = -Wall -Wextra -Wstack-protector -Wformat=2 -Wformat-security
FEATURE= -fdiagnostics-color=always -fno-pic -ffreestanding -fstack-protector-all
CFLAGS = $(WARN) $(FEATURE) -nostdlib -I../include -Ofast -static -s -std=gnu2x -mcmodel=kernel -march=core2 -mno-3dnow -mno-mmx

compile=\
main.c

link=\
../build/installer/main.o

outFileName=installer.elf

include ../.config

.SUFFIXES: .o .c

all: $(compile)
	@x86_64-elf-gcc $(CFLAGS) -
.c.o:
	@echo "CC    $(subst .o,.c,$@) => $@"
	@x86_64-elf-gcc $(CFLAGS) -mno-red-zone -c $(subst .o,.c,$@) -o ../build/installer/$@

clean:
	@rm -f $(link) ../build/installer/*.elf || true

softclean:
	@rm -f $(link) || true
