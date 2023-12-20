WARN=-Wall -Wextra -Wpedantic -Wno-language-extension-token -Wshadow -Wstack-protector -Wformat=2 -Wconversion -Wformat-security -Wunused-parameter -Werror
FEAT=-ffreestanding -fpie -funwind-tables
CFLAGS=-nostdlib -g -I../include $(WARN) $(FEAT) -std=gnu2x

include ../common.mk

all: ../lib/libc.a


../lib/libc.a: $(OBJFILES)
	@mkdir -p $(@D)
	@$(info AR    $(patsubst $(build)/%,%,$^) ==> $(patsubst ../lib/%,%,$@))
	@$(AR) cr $@ $^
	@$(info [32mKernel libc compiled![0m)


$(build)/%.o: %.c
	@mkdir -p $(@D)
	@$(info CC    $< ==> $(patsubst %.c,%.o,$<))
	@$(CC) $(CFLAGS) -c $< -o $@

$(build)/%.o: %.S
	@mkdir -p $(@D)
	@$(info AS    $< ==> $(patsubst %.c,%.o,$<))
	@$(CC) -c $< -o $@
	