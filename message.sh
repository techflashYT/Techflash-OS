#!/bin/bash
# Sorry guys bash is slow as heck but I need it for arrays
VERSION=0.1

messages=(
	"Message 0"
	"Message 1"
	"Message 2"
	"Message 3"
)

usage() {
	echo "Usage: message.sh [message id]"
	echo "
Arguments: 
    -h, --help:     Show this messsage
    --messages:     Show the message IDs and the messages they correspond to
    -v, --version:  Show script version"
}
showMessages() {
	for i in "${!messages[@]}"
	do
		echo -e "Message ${i}: \"${messages[$i]}\""
	done
}



if [ "$1" = "-h" ] || [ "$1" = "--help" ] || [ "$2" != "" ] || [ "$*" = "" ]
then
	usage
elif [ "$1" = "-m" ] || [ "$1" = "--messages" ]
then
	showMessages
elif [ "$1" = "-v" ] || [ "$1" = "--version" ]
then
	echo -e "Techflash OS Message.sh version ${VERSION}, created by Techflash.\r\nYou can find the source code for this script at https://github.com/techflashYT/Techflash-OS/tree/master/message.sh"
else
	usage
fi
