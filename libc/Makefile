ifeq ($(strip $(ARCH)),)
	ARCH=x86_64
endif

CC=$(shell command -v $(ARCH)-elf-gcc 2> /dev/null)

ifeq ($(strip $(CC)),)
$(warning [1;33mCross compiler not detected.  Using system compiler, $(HOSTCC)[0m)
CC=$(HOSTCC)
endif


CC=clang -target x86_64-unknown-none -Weverything -Wno-reserved-macro-identifier -Wno-declaration-after-statement -Wno-cast-qual -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-reserved-identifier

WARN=-Wall -Wextra -Wpedantic -Wshadow -Wstack-protector -Wformat=2 -Wconversion -Wformat-security -Wunused-parameter -Werror
FEAT=-ffreestanding -fpie
CFLAGS=-nostdlib -g -I../include $(WARN) $(FEAT) -std=gnu2x -march=x86-64 -mno-3dnow -mno-mmx -mno-sse -mno-sse2
ifeq ($(ARCH),i386)
CFLAGS += -m32
ASFLAGS += -m32
endif

$(info Using $(CC) as the libc C compiler)

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

OBJFILES=$(patsubst %.S,../build/libc/%.o,$(ARCHASMFILES))  $(patsubst ./%.c,../build/libc/%.o,$(CFILES)) $(patsubst ./%.S,../build/libc/%.o,$(ASMFILES)) $(patsubst %.c,../build/libc/%.o,$(ARCHCFILES))
all: ../lib/libc.a


../lib/libc.a: $(OBJFILES)
	@mkdir -p $(@D)
	@$(info AR    $(patsubst ../build/libc/%,%,$^) ==> $(patsubst ../lib/%,%,$@))
	@$(AR) cr $@ $^
	@$(info [32mKernel libc compiled![0m)


../build/libc/%.o: %.c
	@mkdir -p $(@D)
	@$(info CC    $< ==> $(patsubst %.c,%.o,$<))
	@$(CC) $(CFLAGS) -c $< -o $@

../build/libc/%.o: %.S
	@mkdir -p $(@D)
	@$(info AS    $< ==> $(patsubst %.c,%.o,$<))
	@$(CC) -c $< -o $@
	