#!/bin/sh
rules="--enable=all,style,performance,portability,information,unusedFunction,missingInclude"
include="-Iinclude"
disable="-U__INT8_TYPE__ -U__INT16_TYPE__ -U__INT32_TYPE__ -U__INT64_TYPE__ -U__INTPTR_TYPE__ -U_BSD_RUNE_T_ -U__UINT8_TYPE__ -U__UINTPTR_TYPE__ -U__UINT16_TYPE__ -U__UINT32_TYPE__ -U__UINT64_TYPE__"
cppcheck "$rules" "$include" kernel libc -q "$disable" --template=gcc --suppress=missingIncludeSystem --inline-suppr --force
exit $?