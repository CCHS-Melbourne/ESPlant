#!/bin/bash

# Run this script to install the modules (as symbolic links) in your local Arduino directory
# inside your home directory.
#
# The links mean that changes in this repository directory will
# automatically update the libraries that Arduino uses.

# Bash implementation of realpath, Copyright (c) 2012, Morgan T. Aldridge and BSD licensed
# (thank you Morgan!)
# https://github.com/morgant/realpath/blob/master/realpath
function realpath()
{
	local success=true
	local path="$1"

	# make sure the string isn't empty as that implies something in further logic
	if [ -z "$path" ]; then
		success=false
	else
		# start with the file name (sans the trailing slash)
		path="${path%/}"

		# if we stripped off the trailing slash and were left with nothing, that means we're in the root directory
		if [ -z "$path" ]; then
			path="/"
		fi

		# get the basename of the file (ignoring '.' & '..', because they're really part of the path)
		local file_basename="${path##*/}"
		if [[ ( "$file_basename" = "." ) || ( "$file_basename" = ".." ) ]]; then
			file_basename=""
		fi

		# extracts the directory component of the full path, if it's empty then assume '.' (the current working directory)
		local directory="${path%$file_basename}"
		if [ -z "$directory" ]; then
			directory='.'
		fi

		# attempt to change to the directory
		if ! cd "$directory" &>/dev/null ; then
			success=false
		fi

		if $success; then
			# does the filename exist?
			if [[ ( -n "$file_basename" ) && ( ! -e "$file_basename" ) ]]; then
				success=false
			fi

			# get the absolute path of the current directory & change back to previous directory
			local abs_path="$(pwd -P)"
			cd "-" &>/dev/null

			# Append base filename to absolute path
			if [ "${abs_path}" = "/" ]; then
				abs_path="${abs_path}${file_basename}"
			else
				abs_path="${abs_path}/${file_basename}"
			fi

			# output the absolute path
			echo "$abs_path"
		fi
	fi

	$success
}

LIB_DIR="`dirname $0`"

cd "${LIB_DIR}"
git submodule init
git submodule update

ARDUINO_DIR="$1"
if [ -z "$1" ]; then
    # Try both the likely Arduino home directories
    ARDUINO_DIR=$(realpath "${HOME}/Documents/Arduino")
    if ! [ -d "$ARDUINO_DIR" ]; then
	ARDUINO_DIR="$(realpath ${HOME}/Arduino)"
	if ! [ -d "$ARDUINO_DIR" ]; then
	    echo "Arduino directory not found at <home>/Documents/Arduino or <home>/Arduino."
	    echo
	    echo "If you're using a different Arduino install path, specify the directory on the command line as $0 <Arduino path>"
	    exit 1
	fi
    fi
fi

ARDUINO_DIR=$(realpath "$1")

if ! [ -d "$ARDUINO_DIR" ]; then
    echo "Arduino directory not found at ${ARDUINO_DIR}"
    exit 1
fi

ARDUINO_LIB_DIR="${ARDUINO_DIR}/libraries"
mkdir -p ${ARDUINO_LIB_DIR}

for lib in *; do
    if ! [ -d "$lib" ]; then
	continue
    fi
    DEST="${ARDUINO_LIB_DIR}/$lib"
    if ! [ -d $(realpath "${DEST}") ]; then
	echo "Installing $lib..."
	ln -s "$(realpath $lib)" "${DEST}"
    else
	echo "$lib is already installed, skipping..."
    fi
done

echo "****"
echo "Done. You will need to restart the Arduino IDE before it sees the new libraries."
