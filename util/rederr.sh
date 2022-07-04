#!/bin/bash
while read -r line
do
	printf "\x1b[1;31m%s%s\r\n" "$line" "$(tput sgr0)"
done < "${1:-/dev/stdin}"
