DEFAULT_HOST!=../util/default-host.sh
HOST?=$(DEFAULT_HOST)
HOSTARCH!=../util/target-triplet-to-arch.sh $(HOST)

CFLAGS?=-Ofast
CPPFLAGS?=
LDFLAGS?=-s
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra  -Wpedantic -Werror -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -fstack-protector-all
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude  -Wpedantic -Werror -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -fstack-protector-all
LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk

ARCHDIR=arch/$(HOSTARCH)
include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)

FREEOBJS=\
$(ARCH_FREEOBJS) \
stdio/printf.o \
stdio/puts.o \
stdio/itoa.o \
stdlib/abort.o \
string/memcmp.o \
string/memcpy.o \
string/memmove.o \
string/memset.o \
string/strlen.o \
string/strcmp.o

FREEOBJS_CXX=\
stdio/putchar.o

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

ALL_FREEOBJS=\
$(FREEOBJS_CXX) \
$(FREEOBJS)

OBJS=\
$(FREEOBJS_CXX) \
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(ALL_FREEOBJS:.o=.libk.o)

#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=libk.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .cpp .S
all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
	make softclean

libk.a: $(LIBK_OBJS)
	@echo "AR    $@"
	@$(AR) rcs $@ $(LIBK_OBJS)
	@echo "CLEAN"
	@make softclean --quiet

.cpp.o:
	@echo "CXX   $@"
	@$(CXX) -c $< -o $@ -std=gnu++17 $(CPPFLAGS)

.c.o:
	@echo "CC    $@"
	@$(CC) -c $< -o $@ -std=gnu17 $(CFLAGS) $(CPPFLAGS)

.c.S:
	@echo "CC    $@"
	@$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libk.o:
	@echo "CC    $@"
	@$(CC) -c $< -o $@ -std=gnu17 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.cpp.libk.o:
	@echo "CC    $@"
	@$(CC) -c $< -o $@ -std=gnu++20 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)	

.S.libk.o:
	@echo "CC    $@"
	@$(CC) -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

softclean:
	@rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

clean:
	@rm -f $(BINARIES) *.a
	@rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
