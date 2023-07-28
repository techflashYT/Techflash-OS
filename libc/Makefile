ifeq ($(strip $(ARCH)),)
	ARCH=x86_64
endif

CC=$(shell command -v $(ARCH)-elf-gcc 2> /dev/null)
AR=$(shell command -v $(ARCH)-elf-ar 2> /dev/null)

ifeq ($(strip $(CC)),)
	$(warning [1;33mCross compiler not detected.  Using system compiler, $(HOSTCC)[0m)
	CC=$(HOSTCC)
	AR=$(HOSTCC:gcc=ar)
endif

CFLAGS=-nostdlib -ffreestanding -I../include

$(info Using $(CC) as the libc C compiler)

CFILES       = $(shell find -O3 . -name '*.c' -not -path './arch/*')
ARCHCFILES   = $(shell find -O3 arch/$(ARCH) -name '*.c')
ARCHASMFILES = $(shell find -O3 arch/$(ARCH) -name '*.S')
ASMFILES     = $(shell find -O3 . -name '*.S' -not -path './arch/*')

OBJFILES=$(patsubst ./%.c,../build/libc/%.o,$(CFILES)) $(patsubst ./%.S,../build/libc/%.o,$(ASMFILES)) $(patsubst %.c,../build/libc/%.o,$(ARCHCFILES)) $(patsubst %.S,../build/libc/%.o,$(ARCHASMFILES))
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
	