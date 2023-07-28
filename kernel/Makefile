CC=$(shell command -v x86_64-elf-gcc 2> /dev/null)
ifeq ($(strip $(CC)),)
$(warning [1;33mCross compiler not detected.  Using system compiler, $(HOSTCC)[0m)
CC=$(HOSTCC)
endif

CFLAGS=-nostdlib -ffreestanding

$(info Using $(CC) as the kernel C compiler)

CFILES = $(foreach dir,./,$(notdir $(wildcard $(dir)/*.c)))
ASMFILES = $(foreach dir,./,$(notdir $(wildcard $(dir)/*.S)))

OBJFILES=$(patsubst %.c,../build/kernel/%.o,$(CFILES)) $(patsubst %.S,../build/kernel/%.o,$(ASMFILES))
all: ../bin/tfos_kernel.elf

../bin/tfos_kernel.elf: $(OBJFILES)
	@mkdir -p $(@D)
	@$(info LD    $< ==> $@)
	@$(CC) $(CFLAGS) $^ -o $@

../build/kernel/%.o: %.c
	@mkdir -p $(@D)
	@$(info CC    $< ==> $@)
	@$(CC) $(CFLAGS) -c $< -o $@

../build/kernel/%.o: %.S
	@mkdir -p $(@D)
	@$(info AS    $< ==> $@)
	@$(CC) -c $< -o $@
	