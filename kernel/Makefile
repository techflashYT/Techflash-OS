ifeq ($(strip $(ARCH)),)
	ARCH=x86_64
endif

CC=$(shell command -v $(ARCH)-elf-gcc 2> /dev/null)

ifeq ($(strip $(CC)),)
$(warning [1;33mCross compiler not detected.  Using system compiler, $(HOSTCC)[0m)
CC=$(HOSTCC)
LD=ld.lld
endif

CC=clang -target x86_64-unknown-none -Weverything -Wno-reserved-macro-identifier -Wno-declaration-after-statement -Wno-cast-qual -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-reserved-identifier -Wno-vla -Wno-unsafe-buffer-usage -Wno-padded -Wno-language-extension-token -Wno-cast-align


LIBS=-lc

LDFLAGS=-L../lib -TlinkerScript.ld
WARN=-Wall -Wextra -Wno-pedantic -Wshadow -Wstack-protector -Wformat=2 -Wconversion -Wformat-security -Wunused-parameter -Werror
FEAT=-ffreestanding -fpie -funwind-tables
INCLUDES=-imacros misc/defines.h -include kernel/log.h
CPU_FLAGS=-march=x86-64 -mno-3dnow -mno-mmx -mno-sse -mno-sse2
CFLAGS=-nostdlib -g -I../include $(WARN) $(FEAT) $(INCLUDES) $(CPU_FLAGS) -DGIT_STR=$(GIT_STR) -std=gnu2x -Wno-frame-address
ifeq ($(ARCH),i386)
CFLAGS += -m32
ASFLAGS += -m32
endif

$(shell ../util/parse-config.sh ../)
include ../.config_makefile_parsed

$(info Using $(CC) as the kernel C compiler)

CFILES       = $(shell find -O3 . -name '*.c' -not -path './arch/*')
ARCHCFILES   = $(shell find -O3 arch/$(ARCH) -name '*.c')
ARCHASMFILES = $(shell find -O3 arch/$(ARCH) -name '*.S' | sort)

ifeq ($(ARCH),x86_64)
ARCHCFILES  += $(shell find -O3 arch/x86 -name '*.c')
ARCHASMFILES+= $(shell find -O3 arch/x86 -name '*.S' | sort)
endif
ifeq ($(ARCH),i386)
ARCHCFILES  += $(shell find -O3 arch/x86 -name '*.c')
ARCHASMFILES+= $(shell find -O3 arch/x86 -name '*.S' | sort)
endif

ASMFILES     = $(shell find -O3 . -name '*.S' -not -path './arch/*')

OBJFILES=$(patsubst %.S,../build/kernel/%.o,$(ARCHASMFILES)) $(patsubst ./%.c,../build/kernel/%.o,$(CFILES)) $(patsubst ./%.S,../build/kernel/%.o,$(ASMFILES)) $(patsubst %.c,../build/kernel/%.o,$(ARCHCFILES)) ../build/kernel/font.f16.bin.o
all: ../bin/tfos_kernel.elf

../bin/tfos_kernel.elf: $(OBJFILES)
	@mkdir -p $(@D)
	@$(info LD    $(patsubst ../build/kernel/%.o,%.o,$^) $(foreach lib,$(subst -l,lib,$(LIBS)),$(lib).a) ==> $(patsubst ../bin/%,%,$@))
	@$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

../build/kernel/%.o: %.c
	@mkdir -p $(@D)
	@$(info CC    $< ==> $(patsubst %.c,%.o,$<))
	@$(CC) $(CFLAGS) -c $< -o $@
../build/kernel/font.f16.bin.o: ../util/vgafont.f16
	@$(info PERL  vgafont.f16 ==> font.f16.c)
	@perl ../util/navcc.perl ../util/vgafont.f16 > ../build/kernel/font.f16.c
	@$(info CC    font.f16.c ==> font.bin.f16.o)
	@$(CC) -c -Wno-gnu-binary-literal -std=gnu2x ../build/kernel/font.f16.c -o ../build/kernel/font.f16.bin.o

../build/kernel/%.o: %.S
	@mkdir -p $(@D)
	@$(info AS    $< ==> $(patsubst %.c,%.o,$<))
	@$(CC) $(ASFLAGS) -c $< -o $@
	