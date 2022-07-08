#!/bin/sh
if command -v cppcheck > /dev/null
then
	cppcheck --enable=all,style,performance,portability,information,unusedFunction,missingInclude -I.kernel .
else
	packageCmd=null
	if command -v apt-get > /dev/null
	then
		packageCmd="apt-get update && apt-get install cppcheck -y"
	elif command -v pacman > /dev/null
	then
		packageCmd="pacman -Syyuu && pacman -S cppcheck --no-confirm"
	elif command -v dpkg > /dev/null
	then
		if command -v wget > /dev/null
		then
			packageCmd="wget -O /tmp/cppcheck.deb http://http.us.debian.org/debian/pool/main/c/cppcheck/cppcheck_2.7-1_amd64.deb && dpkg -i /tmp/cppcheck.deb && rm -f /tmp/cppcheck.deb"
		elif command -v curl > /dev/null
		then
			packageCmd="curl http://http.us.debian.org/debian/pool/main/c/cppcheck/cppcheck_2.7-1_amd64.deb --output /tmp/cppcheck.deb && dpkg -i /tmp/cppcheck.deb && rm -f /tmp/cppcheck.deb"
		else
			packageCmd="dpkgFoundButNotWgetOrCurl"
		fi
	else
		echo "ERROR: Cppcheck not found, and we can't find any suitable package manager to download it."
		return 3
	fi
	if [ "$packageCmd" = "dpkgFoundButNotWgetOrCurl" ]
	then
		echo "ERROR: We found a suitable package manager to install Cppcheck, but not any way to download it!  Please download either Wget or cURL, or download Cppcheck itself"
		return 2
	fi
	echo "ERROR: Cppcheck not found!  Install it with \"${packageCmd}\""
	return 1
fi
