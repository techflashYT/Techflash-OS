#!/bin/bash
if [ "$1" = "" ]
then
	echo "This script is not meant to be run standalone!  This is a helper script for the Makefile!"
	exit 2
fi

# Seperate the args into variables
CONFIGURE_KERN_VERSION_MAJOR=$1
CONFIGURE_KERN_VERSION_MINOR=$2
CONFIGURE_KERN_VERSION_PATCH=$3
EXPECTED_KERN_VERSION_MAJOR=$4
EXPECTED_KERN_VERSION_MINOR=$5
EXPECTED_KERN_VERSION_PATCH=$6

if [ "$CONFIGURE_KERN_VERSION_MAJOR" = "$EXPECTED_KERN_VERSION_MAJOR" ] && [ "$CONFIGURE_KERN_VERSION_MINOR" = "$EXPECTED_KERN_VERSION_MINOR" ] && [ "$CONFIGURE_KERN_VERSION_PATCH" = "$EXPECTED_KERN_VERSION_PATCH" ]
then
	printf 0
else
	printf 1
fi

