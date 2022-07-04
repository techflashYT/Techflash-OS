#!/bin/bash
if [ $"$CONFIG_BITS" = "32" ]
then
	echo "i686-elf"
else
	echo "x86_64-elf"
fi
