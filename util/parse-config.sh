#!/bin/bash

# Check if directory argument was provided
if [ -z "$1" ]
then
    echo "No directory argument supplied"
    exit 1
fi

dir=$1
config_file="${dir}/.config"
output_file="${dir}/.config_makefile_parsed"

# Check if .config file exists in the provided directory
if [ ! -f "${config_file}" ]
then
    echo ".config file not found in the provided directory"
    exit 1
fi

# Initialize a buffer variable
buffer=""

# empty the file
echo -n "" > "${output_file}"


start_of_section="true"
# Read .config file line by line
while IFS="" read -r line || [[ -n "$line" ]]; do
    # Check if line is a comment or empty
    if [[ $line = \#* ]] || [[ -z "${line// }" ]]; then
        # If buffer is not empty, write it to output file without backslash
        start_of_section="true"
        if [[ -n "$buffer" ]]; then
            echo "$buffer" >> "${output_file}"
            buffer=""
        fi
        echo "${line}" >> "${output_file}"
    else
        if [ "$start_of_section" = "true" ]; then 
            echo "CFLAGS += \\" >> "${output_file}"
            start_of_section="false"
        fi
        # If buffer is not empty, write it to output file with backslash
        if [[ -n "$buffer" ]]; then
            echo "${buffer} \\" >> "${output_file}"
        fi
        # Set buffer to current line with '-D' at the beginning
        buffer="-D${line}"
    fi
done < "${config_file}"

# Write the last line to output file without backslash
if [[ -n "$buffer" ]]; then
    echo "$buffer" >> "${output_file}"
    echo "" >> "${output_file}"
fi
