LIBS=-lc

LDFLAGS=-L../lib -TlinkerScript.ld
WARN=-Wall -Wextra -Wno-pedantic -Wshadow -Wstack-protector -Wformat=2 \
     -Wconversion -Wformat-security -Wunused-parameter -Werror

FEAT=-ffreestanding -fpie -funwind-tables
INCLUDES=-I../include -imacros misc/defines.h -include kernel/log.h
CFLAGS=-nostdlib -g $(WARN) $(FEAT) $(INCLUDES) -DGIT_STR=$(GIT_STR) -std=gnu2x

include ../common.mk
OBJFILES += $(build)/font.f16.bin.o

all: ../bin/tfos_kernel.elf

../bin/tfos_kernel.elf: $(OBJFILES)
	@mkdir -p $(@D)
	@$(info LD    $(patsubst $(build)/%.o,%.o,$^) $(foreach lib,$(subst -l,lib,$(LIBS)),$(lib).a) ==> $(patsubst ../bin/%,%,$@))
	@$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(build)/%.o: %.c
	@mkdir -p $(@D)
	@$(info CC    $< ==> $(patsubst %.c,%.o,$<))
	@$(CC) $(CFLAGS) -c $< -o $@
$(build)/font.f16.bin.o: ../util/vgafont.f16
	@$(info PERL  vgafont.f16 ==> font.f16.c)
	@perl ../util/navcc.perl ../util/vgafont.f16 > $(build)/font.f16.c
	@$(info CC    font.f16.c ==> font.bin.f16.o)
	@$(CC) -c -Wno-gnu-binary-literal -std=gnu2x $(build)/font.f16.c -o $(build)/font.f16.bin.o

$(build)/%.o: %.S
	@mkdir -p $(@D)
	@$(info AS    $< ==> $(patsubst %.c,%.o,$<))
	@$(CC) $(ASFLAGS) -c $< -o $@
	