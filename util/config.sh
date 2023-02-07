#!/bin/sh
SYSTEM_HEADER_PROJECTS="libc kernel userspaceLibcWrappers"
PROJECTS="libc kernel userspaceLibcWrappers"

export MAKE="${MAKE:-make --no-print-directory}"
export HOST="${HOST:-"x86_64-elf"}"

export AR="${HOST}"-ar
export AS="${HOST}"-as
export CC="${CC:-${HOST}-gcc}"
export LD="${HOST}"-ld
export CXX="${HOST}"-g++

export PROJECTS
export SYSTEM_HEADER_PROJECTS

# Configure the cross-compiler to use the desired system root.
SYSROOT="$(pwd)/sysroot"
export SYSROOT
export CC="$CC --sysroot=$SYSROOT"
export CXX="$CXX --sysroot=$SYSROOT"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
	export CC="$CC -isystem=$INCLUDEDIR"
	export CXX="$CXX -isystem=$INCLUDEDIR"
fi
