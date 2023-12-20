# If not specified, build for the current systen's arch
ifeq ($(strip $(ARCH)),)
	ARCH=$(shell uname -m)
endif

# Set our C compiler
CC=$(shell command -v $(ARCH)-elf-gcc 2> /dev/null)


# DEPRECATION: I haven't tested the cross compiler build in ages and it's pretty stupid
# we always just use clang instead
# ifeq ($(strip $(CC)),)
# $(warning [1;33mCross compiler not detected.  Using system compiler, $(HOSTCC)[0m)
# CC=$(HOSTCC)
# endif


CC=clang
$(info Using $(CC) as C compiler.)

# this is here just in case I ever decide to not use clang
# makes it more compiler agnostic, to not use clang-specific
# flags by default
ifeq ($(CC),clang)
CFLAGS += -target $(ARCH)-unknown-none -Weverything -Wno-reserved-macro-identifier \
-Wno-declaration-after-statement -Wno-cast-qual -Wno-missing-prototypes \
-Wno-missing-variable-declarations -Wno-reserved-identifier -Wno-unsafe-buffer-usage \
-Wno-cast-align -Wno-vla
endif


# parse the .config, and include it
$(shell ../util/parse-config.sh ../)
include ../.config_makefile_parsed



$(info Using $(CC) as the libc C compiler)

# find the source code
CFILES       = $(shell find -O3 . -name '*.c' -not -path './arch/*')
ARCHCFILES   = $(shell find -O3 arch/$(ARCH) -name '*.c')
ASMFILES = $(shell find -O3 arch/$(ARCH) -name '*.S' | sort)

# x86 specific flags
ifeq ($(ARCH),x86_64)
generic_x86_stuff=y
else ifeq ($(ARCH),i386)
generic_x86_stuff=y
CFLAGS  += -m32
ASFLAGS += -m32
endif

ifeq ($(generic_x86_stuff),y)
CFLAGS      += -march=x86-64 -mno-3dnow -mno-mmx -mno-sse -mno-sse2

# find source code that is for any x86 CPU
ARCHCFILES  += $(shell find -O3 arch/x86 -name '*.c')
ASMFILES    += $(shell find -O3 arch/x86 -name '*.S' | sort)
endif


# set the OBJFILES
proj=$(shell basename $$PWD)
build=../build/$(proj)
OBJFILES=$(patsubst %.S,$(build)/%.o,$(ASMFILES))  $(patsubst ./%.c,$(build)/%.o,$(CFILES)) $(patsubst %.c,$(build)/%.o,$(ARCHCFILES))
