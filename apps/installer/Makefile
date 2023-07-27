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
	@$(CC) $(CFLAGS) -
.c.o:
	@echo "CC    $(subst .o,.c,$@) => $@"
	@$(CC) $(CFLAGS) -mno-red-zone -c $(subst .o,.c,$@) -o ../build/installer/$@

clean:
	@rm -f $(compile) ../build/installer/*.elf || true

softclean:
	@rm -f $(compile) || true
