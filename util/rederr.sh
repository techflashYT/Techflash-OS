#!/bin/bash
while read -r line
do
	line=$(echo "$line" | sed 's/^.*ld\:/ld:/;s/\.\.\///g') # Remove the absurdly long prefix to the ld binary & remove the '..' from the path the the build directories
	if [[ "$line" = *"undefined reference to"* ]] || [[ "$line" = *"multiple defintions of"* ]] || [[ "$line" = *"cannot find"* ]] || [[ "$line" = *"collect2: error:"* ]];then
		printf "\x1b[1;31m%s%s\r\n" "$line" "$(tput sgr0)"
	elif [[ "$line" = *"warning"* ]];then
		printf "\x1b[1;35m%s%s\r\n" "$line" "$(tput sgr0)"
	else
		printf "$line\r\n"
	fi
done < "${1:-/dev/stdin}"
