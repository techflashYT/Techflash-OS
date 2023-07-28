CC=$(shell command -v x86_64-elf-gcc 2> /dev/null)
ifeq ($(strip $(CC)),)
$(warning [1;33mCross compiler not detected.  Using system compiler, $(HOSTCC)[0m)
CC=$(HOSTCC)
endif

CFLAGS=-nostdlib -ffreestanding

$(info Using $(CC) as the libc C compiler)

CFILES = $(foreach dir,./,$(notdir $(wildcard $(dir)/*.c)))
ASMFILES = $(foreach dir,./,$(notdir $(wildcard $(dir)/*.S)))

OBJFILES=$(patsubst %.c,../build/libc/%.o,$(CFILES)) $(patsubst %.S,../build/libc/%.o,$(ASMFILES))
all: ../lib/libc.a


../lib/libc.a: $(compile)
	@mkdir -p $(@D)
	@$(info AR    $< ==> $@)
	@$(AR) cr $< $@
	@$(info [32mKernel libc compiled![0m)


../build/libc/%.o: %.c
	@mkdir -p $(@D)
	@$(info CC    $< ==> $@)
	@$(CC) $(CFLAGS) -c $< -o $@

../build/libc/%.o: %.S
	@mkdir -p $(@D)
	@$(info AS    $< ==> $@)
	@$(CC) -c $< -o $@
	