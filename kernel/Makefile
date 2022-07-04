DEFAULT_HOST!=../util/default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../util/target-triplet-to-arch.sh $(HOST)

CFLAGS?=-Ofast -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Wpedantic -Werror -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -fstack-protector-all
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude -Wpedantic -Werror -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -fstack-protector-all
LDFLAGS:=$(LDFLAGS)
ifeq ($(strip $(LIBS)),)
    LIBS:=-nostdlib -lk -lgcc
else
    LIBS:=$(LIBS)-nostdlib -lk -lgcc
endif
ARCHDIR=arch/$(HOSTARCH)
CC:=$(CC)
include $(ARCHDIR)/make.config
EXPECTED_CONFIG_KERN_VERSION_MAJOR=0
EXPECTED_CONFIG_KERN_VERSION_MINOR=0
EXPECTED_CONFIG_KERN_VERSION_PATCH=6
CONFIG_KERN_VERSION_MAJOR=NULL
CONFIG_KERN_VERSION_MINOR=NULL
CONFIG_KERN_VERSION_PATCH=NULL
CONFIG_BITS=NULL

include ../.config
CFLAGS:=$(CFLAGS) -DCONFIG_KERN_VERSION_MAJOR=$(CONFIG_KERN_VERSION_MAJOR) -DCONFIG_KERN_VERSION_MINOR=$(CONFIG_KERN_VERSION_MINOR) -DCONFIG_KERN_VERSION_PATCH=$(CONFIG_KERN_VERSION_PATCH) -DCONFIG_KERN_MAXARGS=$(CONFIG_KERN_MAXARGS) -DCONFIG_BITS=$(CONFIG_BITS)
CPPFLAGS:=$(CPPFLAGS)
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS)\
kernel/kernel.o \
kernel/stack.o \
kernel/panic.o \
kernel/bootDisplay.o

OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

LINK_LIST=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS)\
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
$(LIBS)

clean:
	@rm -f tfos.elf
	@rm -f $(OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

.PHONY: all install install-headers install-kernel
.SUFFIXES: .o .c .S .S64

all: tfos.elf
RET := $(shell ./testversion.sh $(CONFIG_KERN_VERSION_MAJOR) $(CONFIG_KERN_VERSION_MINOR) $(CONFIG_KERN_VERSION_PATCH) $(EXPECTED_CONFIG_KERN_VERSION_MAJOR) $(EXPECTED_CONFIG_KERN_VERSION_MINOR) $(EXPECTED_CONFIG_KERN_VERSION_PATCH))
ifneq ($(strip $(RET)),0)
$(error Kernel Version Mismatch!  Expected: $(EXPECTED_CONFIG_KERN_VERSION_MAJOR).$(EXPECTED_CONFIG_KERN_VERSION_MINOR).$(EXPECTED_CONFIG_KERN_VERSION_PATCH)  Got: $(CONFIG_KERN_VERSION_MAJOR).$(CONFIG_KERN_VERSION_MINOR).$(CONFIG_KERN_VERSION_PATCH) Error code: $(RET))
endif

tfos.elf: $(OBJS) $(ARCHDIR)/linker.ld
	@echo "LINK $(LDFLAGS) $(LINK_LIST) ===> $@"
	$(CC) -T $(ARCHDIR)/linker.ld -mno-red-zone -o $@ $(CFLAGS) $(LDFLAGS) $(LINK_LIST)
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	@cp tfos.elf $(DESTDIR)$(BOOTDIR)
# @grub-file --is-x86-multiboot2 ../sysroot/boot/tfos.elf
	@echo "CLEAN"
	@make softclean --quiet

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	@OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	@echo "CC    $@"
	@$(CC) -c $< -o $@ -std=gnu2x $(CFLAGS) $(CPPFLAGS)

.S64.o:
	@echo "CC    $@"
	@$(CC) -x assembler -c $< -o $@ $(CFLAGS) $(CPPFLAGS)



install: install-headers install-kernel

install-headers:
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: tfos.elf
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	@cp tfos.elf $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
softclean:
	@rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

