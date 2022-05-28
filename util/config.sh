#!/bin/sh
SYSTEM_HEADER_PROJECTS="userspace libc kernel"
PROJECTS="userspace libc kernel"

export MAKE="${MAKE:-make}"
export HOST="${HOST:-$(./util/default-host.sh)}"

export AR="${HOST}"-ar
export AS="${HOST}"-as
export CC="${HOST}"-gcc
export CXX="${HOST}"-g++

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-Ofast'
export CPPFLAGS=''
export PROJECTS
export SYSTEM_HEADER_PROJECTS

# Configure the cross-compiler to use the desired system root.
SYSROOT="$(pwd)/sysroot"
export SYSROOT
export CC="$CC --sysroot=$SYSROOT"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
  export CC="$CC -isystem=$INCLUDEDIR"
fi
