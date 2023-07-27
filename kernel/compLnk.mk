# compile=$(addprefix ../build/kernel/,$(subst .S,.o,$(subst .psf,.o,$(subst .c,.o,$(subst ./,,$(shell find -O3 . -type f | grep -v "\.md" | grep -v "Makefile" | grep -v "\.ld" | grep -v "\.mk" | grep -v "entryPoint"))))))

override compile_S   := $(shell find -O3 . -type f -name '*.S'    | grep -v "__disabled_file")
override compile_c   := $(shell find -O3 . -type f -name '*.c'    | grep -v "__disabled_file")
override compile_psf := $(shell find -O3 . -type f -name '*.psf'  | grep -v "__disabled_file")
compile=$(addprefix ../build/kernel/,$(subst ./,,$(subst .psf,.o,$(subst .c,.o,$(subst .S,.o,$(compile_S) $(compile_c) $(compile_psf)))))) ../build/kernel/font.o
