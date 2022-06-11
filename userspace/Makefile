DEFAULT_HOST!=../util/default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../util/target-triplet-to-arch.sh $(HOST)

CFLAGS?=-Ofast -g
CPPFLAGS?=
LDFLAGS?=-s
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra  -Wpedantic -Werror -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -fstack-protector-all
CPPFLAGS:=$(CPPFLAGS) -Iinclude  -Wpedantic -Werror -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -fstack-protector-all
USERSPACE_CFLAGS:=$(CFLAGS)
USERSPACE_CPPFLAGS:=$(CPPFLAGS)

ARCHDIR=arch/$(HOSTARCH)
include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
USERSPACE_CFLAGS:=$(USERSPACE_CFLAGS) $(KERNEL_ARCH_CFLAGS)
USERSPACE_CPPFLAGS:=$(USERSPACE_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)

FREEOBJS=\
$(ARCH_FREEOBJS) \
apps/shell/shell.o

FREEOBJS_CXX=\
apps/terminal/term.o

ALL_FREEOBJS=\
$(FREEOBJS_CXX) \
$(FREEOBJS)

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS_CXX) \
$(FREEOBJS) \
$(HOSTEDOBJS)

USERSPACE_OBJS=$(ALL_FREEOBJS:.o=.libuser.o)

BINARIES=libuser.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libuser.o .c .cpp

all: $(BINARIES)

libuser.a: $(USERSPACE_OBJS)
	@$(AR) rcs $@ $(USERSPACE_OBJS)
	@echo "CLEAN"
	@make softclean --quiet

.cpp.o:
	@echo "CXX    $@"
	@$(CXX) -c $< -o $@ -std=gnu++2b $(CPPFLAGS)

.c.o:
	@echo "CC     $@"
	@$(CC) -c $< -o $@ -std=gnu2x $(CFLAGS) $(CPPFLAGS)

.cpp.libuser.o:
	@echo "CXX    $@"
	@$(CXX) -c $< -o $@ -std=gnu++2b $(USERSPACE_CFLAGS) $(USERSPACE_CPPFLAGS)

.c.libuser.o:
	@echo "CC     $@"
	@$(CC) -c $< -o $@ -std=gnu2x $(USERSPACE_CFLAGS) $(USERSPACE_CPPFLAGS)

.S.libuser.o:
	@echo "CC     $@"
	@$(CC) -c $< -o $@ $(USERSPACE_CFLAGS) $(USERSPACE_CPPFLAGS)

softclean:
	@rm -f $(OBJS) $(USERSPACE_OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) $(USERSPACE_OBJS:.o=.d) *.d */*.d */*/*.d

clean:
	@rm -f $(BINARIES) *.a
	@rm -f $(OBJS) $(USERSPACE_OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) $(USERSPACE_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(USERSPACE_OBJS:.o=.d)
