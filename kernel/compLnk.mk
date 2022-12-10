# compile=$(addprefix ../build/kernel/,$(subst .S,.o,$(subst .psf,.o,$(subst .c,.o,$(subst ./,,$(shell find . -type f | grep -v "\.md" | grep -v "Makefile" | grep -v "\.ld" | grep -v "\.mk" | grep -v "entryPoint"))))))

override compile_S   := $(shell find . -type f -name '*.S')
override compile_c   := $(shell find . -type f -name '*.c')
override compile_psf := $(shell find . -type f -name '*.psf')
compile=$(addprefix ../build/kernel/,$(subst ./,,$(subst .psf,.o,$(subst .c,.o,$(subst .S,.o,$(compile_S) $(compile_c) $(compile_psf))))))
