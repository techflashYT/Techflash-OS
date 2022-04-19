#!/bin/sh
if [ "$1" = "1" ]
then
    printf "\033[1;32mSTEP 1/4: Bootloader compiled!\033[0m\r\n"
elif [ "$1" = "2" ]
then
    printf "\033[1;32mSTEP 2/4: C standard library compiled!\033[0m\r\n"
elif [ "$1" = "3" ]
then
    printf "\033[1;32mSTEP 3/4: Kernel compiled!\033[0m\r\n"
elif [ "$1" = "4" ]
then
    printf "\033[1;32mSTEP 4/4: Linked!\033[0m\r\n"
elif [ "$1" = "5" ]
then
    printf "\033[1;32mFully compiled in bin/bootsect.bin!\033[0m\r\n"
elif [ "$1" = "6" ]
then
    printf "\033[1;33mCompilin C Standard library to a static library.\r\nThis more than likely will fail.\033[0m\r\n"
elif [ "$1" = "e" ]
then
    printf "\033[1;31m"
elif [ "$1" = "r" ]
    tput sgr0
else
    printf "\033[1;31mERROR: Invalid argument!\033[0m\r\n"
fi

