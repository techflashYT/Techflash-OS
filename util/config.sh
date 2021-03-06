#!/bin/sh
SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel"

export MAKE="${MAKE:-make --no-print-directory}"
export HOST="${HOST:-$(./util/default-host.sh)}"
export HOST32="${HOST32:-$(CONFIG_BITS=32 ./util/default-host.sh)}"

export AR="${HOST}"-ar
export AS="${HOST}"-as
export CC="${HOST}"-gcc
export LD="${HOST}"-ld
export CXX="${HOST}"-g++

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-Ofast -g -mcmodel=kernel'
export CPPFLAGS=''
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
