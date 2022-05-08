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
    -v, --version:  Show script version
	-m <id>, --message <id>: Show the message with the given ID"
}
showMessages() {
	for i in "${!messages[@]}"
	do
		echo -e "Message ${i}: \"${messages[$i]}\""
	done
}



if [ "$1" = "-h" ] || [ "$1" = "--help" ] || [ "$3" != "" ] || [ "$*" = "" ]
then
	usage
elif [ "$1" = "--messages" ]
then
	if [ "$2" != "" ]
	then
		echo "Invalid argument"
		usage
	else
		showMessages
	fi
elif [ "$1" = "-v" ] || [ "$1" = "--version" ]
then
	if [ "$2" != "" ]
	then
		echo "Invalid argument"
		usage
	else
		echo -e "Techflash OS Message.sh version ${VERSION}, created by Techflash.\r\nYou can find the source code for this script at https://github.com/techflashYT/Techflash-OS/tree/master/message.sh"
	fi
elif [ "$1" = "-m" ] || [ "$1" = "--message" ]
then
	if [ "$2" = "" ]
	then
		echo "You must specify a message ID for -m or --message."
		usage
	elif [ "$2" = "e" ]
	then
		echo "${ERROR}"
	elif [ "$2" = "n" ]
	then
		echo "${NORMAL}"
	elif [[ "$2" =~ ^[0-9]+$ ]] # Test if the argument is a number
	then
		if [ "$2" -ge 0 ] && [ "$2" -lt ${#messages[@]} ]
		then
			echo "${messages[$2]}"
		else
			echo "The message ID you specified is not valid."
			usage
		fi
	else
		echo "The message ID you specified is not valid."
		usage
	fi
else
	usage
fi
