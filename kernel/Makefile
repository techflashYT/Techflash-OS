DEFAULT_HOST!=../util/default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../util/target-triplet-to-arch.sh $(HOST)

CFLAGS?=-Ofast
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
LIBS:=$(LIBS)-nostdlib -lk -lgcc -luser
ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config
EXPECTED_CONFIG_KERN_VERSION_MAJOR=0
EXPECTED_CONFIG_KERN_VERSION_MINOR=0
EXPECTED_CONFIG_KERN_VERSION_PATCH=4
CONFIG_KERN_VERSION_MAJOR=NULL
CONFIG_KERN_VERSION_MINOR=NULL
CONFIG_KERN_VERSION_PATCH=NULL

include ../.config
CFLAGS:=$(CFLAGS) -DCONFIG_KERN_VERSION_MAJOR=$(CONFIG_KERN_VERSION_MAJOR) -DCONFIG_KERN_VERSION_MINOR=$(CONFIG_KERN_VERSION_MINOR) -DCONFIG_KERN_VERSION_PATCH=$(CONFIG_KERN_VERSION_PATCH) -DCONFIG_KERN_MAXARGS=$(CONFIG_KERN_MAXARGS)
CPPFLAGS:=$(CPPFLAGS)
LDFLAGS:=$(LDFLAGS)-s
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS)\
kernel/kernel.o \
kernel/stack.o \
kernel/panic.o \
kernel/debug.o \
kernel/hardware/cpu/GDT.o

KERNEL_CXX_OBJS=\

OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(KERNEL_CXX_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(KERNEL_CXX_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

clean:
	@rm -f tfos.elf
	@rm -f $(OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

.PHONY: all install install-headers install-kernel
.SUFFIXES: .o .c .cpp .S

all: tfos.elf
RET := $(shell ./testversion.sh $(CONFIG_KERN_VERSION_MAJOR) $(CONFIG_KERN_VERSION_MINOR) $(CONFIG_KERN_VERSION_PATCH) $(EXPECTED_CONFIG_KERN_VERSION_MAJOR) $(EXPECTED_CONFIG_KERN_VERSION_MINOR) $(EXPECTED_CONFIG_KERN_VERSION_PATCH))
ifneq ($(strip $(RET)),0)
$(error Kernel Version Mismatch!  Expected: $(EXPECTED_CONFIG_KERN_VERSION_MAJOR).$(EXPECTED_CONFIG_KERN_VERSION_MINOR).$(EXPECTED_CONFIG_KERN_VERSION_PATCH)  Got: $(CONFIG_KERN_VERSION_MAJOR).$(CONFIG_KERN_VERSION_MINOR).$(CONFIG_KERN_VERSION_PATCH) Error code: $(RET))
endif

tfos.elf: $(OBJS) $(ARCHDIR)/linker.ld
	@echo "LINK   $(LINK_LIST)    $@"
	@$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	@cp tfos.elf $(DESTDIR)$(BOOTDIR)
	@grub-file --is-x86-multiboot2 ../sysroot/boot/tfos.elf
	@echo "CLEAN"
	@make softclean --quiet

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	@OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.cpp.o:
	@echo "CXX   $@"
	@$(CXX) -c $< -o $@ -std=gnu++20 $(CFLAGS) $(CPPFLAGS)
.c.o:
	@echo "CC    $@"
	@$(CC) -c $< -o $@ -std=gnu17 $(CFLAGS) $(CPPFLAGS)

.S.o:
	@echo "CC    $@"
	@$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)



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

