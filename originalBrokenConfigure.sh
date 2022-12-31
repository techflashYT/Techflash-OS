#!/bin/bash
# TODO: Make this less trash
CONFIGURE_VERSION=0.1.0
TFOS_VERSION_MAJOR=1
TFOS_VERSION_MINOR=0
TFOS_VERSION_PATCH=0
TFOS_VERSION=$TFOS_VERSION_MAJOR.$TFOS_VERSION_MINOR.$TFOS_VERSION_PATCH
printf "Techflash OS Configure Script v%s starting, built for Techflash OS v%s...\r\n" $CONFIGURE_VERSION $TFOS_VERSION

global_done=false
kernel_done=false
libc_done=false
grub_done=false
end_done=false
if [ "$1" != "--no-max-screen" ]; then
	if [ "$(tput cols)" -lt 100 ]; then
		echo "Your terminal is too small. Please resize it to at least 100 columns."
		echo "If you can't (like if you have basic linux console),"
		echo "please run the script with --no-max-screen"
		exit 1
	fi
	if [ "$(tput lines)" -lt 10 ]; then
		echo "Your terminal is too small. Please resize it to at least 10 lines."
		exit 1
	fi
else
	echo "This option is not supported!!!"
	echo "Be warned.  Things will look weird."
fi
global() {
	printf "======================GLOBAL=====================\r\n"
	
	global_done=true
	return 0
}
grub() {
	printf "======================GRUB=======================\r\n"
	printf "CONFIG_GRUB_OUTER_BG_COL       Background color (GRUB color name) for around the boot menu, (default: black): "
	read -r CONFIG_GRUB_OUTER_BG_COL
	if [ -z "$CONFIG_GRUB_OUTER_BG_COL" ]; then
		CONFIG_GRUB_OUTER_BG_COL="black"
	fi
	if ! {
		[ "$CONFIG_GRUB_OUTER_BG_COL" = "black" ] ||
		[ "$CONFIG_GRUB_OUTER_BG_COL" = "blue" ] ||
		[ "$CONFIG_GRUB_OUTER_BG_COL" = "green" ] ||
		[ "$CONFIG_GRUB_OUTER_BG_COL" = "cyan" ] ||
		[ "$CONFIG_GRUB_OUTER_BG_COL" = "red" ] ||
		[ "$CONFIG_GRUB_OUTER_BG_COL" = "magenta" ] ||
		[ "$CONFIG_GRUB_OUTER_BG_COL" = "brown" ] ||
		[ "$CONFIG_GRUB_OUTER_BG_COL" = "light-gray" ]
	}; then
		clear
		echo "Invalid color name. Please use a valid color name."
		return 1
	fi
	printf "CONFIG_GRUB_OUTER_FG_COL       Foreground color (GRUB color name) for around the boot menu, (default: white): "
	read -r CONFIG_GRUB_OUTER_FG_COL
	if [ -z "$CONFIG_GRUB_OUTER_FG_COL" ]; then
		CONFIG_GRUB_OUTER_FG_COL="white"
	fi
	if ! {
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "black" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "blue" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "green" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "cyan" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "red" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "magenta" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "brown" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "light-gray" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "dark-gray" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "light-blue" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "light-green" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "light-cyan" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "light-red" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "light-magenta" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "yellow" ] ||
		[ "$CONFIG_GRUB_OUTER_FG_COL" = "white" ]
	}; then
		echo "Invalid color name. Please use a valid color name."
		return 1
	fi
	printf "CONFIG_GRUB_MENU_BG_COL        Background color (GRUB color name) for the boot menu, (default: light-gray): "
	read -r CONFIG_GRUB_MENU_BG_COL
	if [ -z "$CONFIG_GRUB_MENU_BG_COL" ]; then
		CONFIG_GRUB_MENU_BG_COL="light-gray"
	fi
	if ! {
		[ "$CONFIG_GRUB_MENU_BG_COL" = "black" ] ||
		[ "$CONFIG_GRUB_MENU_BG_COL" = "blue" ] ||
		[ "$CONFIG_GRUB_MENU_BG_COL" = "green" ] ||
		[ "$CONFIG_GRUB_MENU_BG_COL" = "cyan" ] ||
		[ "$CONFIG_GRUB_MENU_BG_COL" = "red" ] ||
		[ "$CONFIG_GRUB_MENU_BG_COL" = "magenta" ] ||
		[ "$CONFIG_GRUB_MENU_BG_COL" = "brown" ] ||
		[ "$CONFIG_GRUB_MENU_BG_COL" = "light-gray" ]
	}; then
		clear
		echo "Invalid color name. Please use a valid color name."
		return 1
	fi
	printf "CONFIG_GRUB_MENU_FG_COL        Foreground color (GRUB color name) for the boot menu, (default: white): "
	read -r CONFIG_GRUB_MENU_FG_COL
	if [ -z "$CONFIG_GRUB_MENU_FG_COL" ]; then
		CONFIG_GRUB_MENU_FG_COL="white"
	fi
	if ! {
		[ "$CONFIG_GRUB_MENU_FG_COL" = "black" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "blue" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "green" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "cyan" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "red" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "magenta" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "brown" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "light-gray" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "dark-gray" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "light-blue" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "light-green" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "light-cyan" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "light-red" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "light-magenta" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "yellow" ] ||
		[ "$CONFIG_GRUB_MENU_FG_COL" = "white" ]
	}; then
		clear
		echo "Invalid color name. Please use a valid color name."
		return 1
	fi
	grub_done=true
	return 0
}
kernel() {
	printf "=====================KERNEL======================\r\n"
	
	printf "CONFIG_KERN_MAXARGS            The maximum number of arguments that the kernel will allocate space for, 0 - 65535 (default: 250): "
	read -r CONFIG_KERN_MAXARGS
	if [ -z "$CONFIG_KERN_MAXARGS" ]; then
		CONFIG_KERN_MAXARGS=250
	fi
	if ! [[ $CONFIG_KERN_MAXARGS =~ ^[0-9]+$ ]]; then
		clear
		echo "Not a number!  Please choose a valid number between 0 and 65535"
		return 1
	fi
	if [ $CONFIG_KERN_MAXARGS -lt 0 ] || [ $CONFIG_KERN_MAXARGS -gt 65535 ]; then
		clear
		echo "Invalid number, please choose a value between 0 and 65535"
		return 1
	fi
	kernel_done=true
	return 0
}
libc() {
	printf "===================LIBC/LIBK=====================\r\n"
	libc_done=true
	return 0
}
edit() {
	clear
	printf "\
What would you like to edit?\r\n\
\x1b[9m1. Global\x1b[0m\r\n\
2. GRUB\r\n\
\x1b[9m3. Kernel\x1b[0m\r\n\
\x1b[9m4. libc/libk\x1b[0m\r\n\
5. Review\r\n"
	read -r editChoice
	case "$editChoice" in
		1)
			clear
			printf "\x1b[33mSorry!  This part doesn't have any configuration options yet!\x1b[0m\r\n"
			return 1;;
		2)
			grub;;
		3)
			clear
			printf "\x1b[33mSorry!  This part doesn't have any configuration options yet!\x1b[0m\r\n"
			return 1;;
		4)
			clear
			printf "\x1b[33mSorry!  This part doesn't have any configuration options yet!\x1b[0m\r\n"
			return 1;;
		5)
			end
			return 0;;
		*)
			clear
			printf "\x1b[31mInvalid option!  Please try again!\x1b[0m"
			return 1;;
	esac
}
end() {
	HEADER=$(cat << EOF
# Config file generated by Techflash OS Configure Script v$CONFIGURE_VERSION
# Made for Techflash OS v$TFOS_VERSION
# !!! DO NOT MANUALLY EDIT THIS FILE !!! USE THE CONFIGURE SCRIPT !!!
###### START AUTOGENERATED CONFIG ######
EOF
)
	CONFIG=$(cat << EOF
CONFIGURE_VERSION=$CONFIGURE_VERSION
CONFIG_KERN_VERSION_MAJOR=$TFOS_VERSION_MAJOR
CONFIG_KERN_VERSION_MINOR=$TFOS_VERSION_MINOR
CONFIG_KERN_VERSION_PATCH=$TFOS_VERSION_PATCH
CONFIG_KERN_MAXARGS=$CONFIG_KERN_MAXARGS
CONFIG_GRUB_OUTER_BG_COL=$CONFIG_GRUB_OUTER_BG_COL
CONFIG_GRUB_OUTER_FG_COL=$CONFIG_GRUB_OUTER_FG_COL
CONFIG_GRUB_MENU_BG_COL=$CONFIG_GRUB_MENU_BG_COL
CONFIG_GRUB_MENU_FG_COL=$CONFIG_GRUB_MENU_FG_COL
EOF
)
	FOOTER=$(cat << EOF
####### END AUTOGENERATED CONFIG #######
EOF
)	
	printf "\r\n\r\n=====================CONFIG======================\r\n"
	printf "$CONFIG\r\n"
	printf "Does this configuration look good? (Y/n): "
	read -r confirm
	if [ -z "$confirm" ]; then
		confirm="y"
	fi
	# Check that confirm is equal to y or Y
	if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
		printf "Your configuration will be saved in %s/.config\r\n" "$(pwd)"
		cat << EOF > "$(pwd)/.config"
$HEADER
$CONFIG
$FOOTER
EOF
		end_done=true
		return 0
	fi
	if [ "$confirm" = "n" ] || [ "$confirm" = "N" ]; then
		if ! edit; then
			edit
		else
			return 1
		fi
	else
		clear
		echo "Invalid input. Please enter y or n."
		end
	fi
}
run() {
	if [ "$global_done" != true ] || ! global; then
		run
	elif [ "$grub_done" != true ] || ! grub; then
		run
	elif [ "$kernel_done" != true ] || ! kernel; then
		run
	elif [ "$libc_done" != true ] || ! libc; then
		run
	elif [ "$end_done" != true ] || ! end; then
		run
	else
		exit 0
	fi
}
run
